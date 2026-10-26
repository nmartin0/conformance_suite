/*
 * pthread_rwlock_tryrdlock() fails with EBUSY while a writer holds the lock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_rdlock.002
 * MUTANT    pthread_rwlock_rdlock.tryrd_blocks
 * MUTANT    pthread_rwlock_rdlock.tryrd_ignores_writer
 * MUTANT    pthread_rwlock_rdlock.tryrd_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero; 1 if it did. */
static int wait_flag(atomic_int *flag, long ms)
{
	long waited = 0;

	while (atomic_load(flag) == 0 && waited < ms) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(flag) != 0;
}

/* Waits up to ms milliseconds for *v to exceed old; 1 if it did. */
static int wait_flag_above(atomic_int *v, int old, long ms)
{
	long waited = 0;

	while (atomic_load(v) <= old && waited < ms) {
		sleep_ms(5);
		waited += 5;
	}
	return atomic_load(v) > old;
}

/*
 * A job: fn(arg) run in a thread of its own. Threads report only through
 * these atomic fields, never through the locks under test.
 */
struct job {
	int (*fn)(void *);
	void *arg;
	atomic_int started, done;
	int result;
	pthread_t thread;
};

static void *job_main(void *p)
{
	struct job *j = p;

	atomic_store(&j->started, 1);
	j->result = j->fn(j->arg);
	atomic_store(&j->done, 1);
	return NULL;
}

/* Starts fn(arg) in a new thread and waits until it has started. */
static void job_start(struct job *j, int (*fn)(void *), void *arg)
{
	j->fn = fn;
	j->arg = arg;
	atomic_store(&j->started, 0);
	atomic_store(&j->done, 0);
	j->result = -1;
	NEED(pthread_create(&j->thread, NULL, job_main, j) == 0, "pthread_create failed");
	NEED(wait_flag(&j->started, 10000), "the new thread did not start");
}

/* Waits for a job that must finish (UNRESOLVED if it does not in 10 s). */
static int job_finish(struct job *j, const char *what)
{
	if (!wait_flag(&j->done, 10000))
		pc_unresolved("%s did not return within 10 s", what);
	NEED(pthread_join(j->thread, NULL) == 0, "pthread_join failed");
	return j->result;
}

/*
 * Runs fn(arg), a call that never waits, in another thread and returns
 * its result; one that has not returned in 10 s is a failed check.
 */
static int in_other_thread(int (*fn)(void *), void *arg, const char *what)
{
	struct job j;

	job_start(&j, fn, arg);
	pc_check(wait_flag(&j.done, 10000), "%s in another thread did not return within 10 s", what);
	NEED(pthread_join(j.thread, NULL) == 0, "pthread_join failed");
	return j.result;
}

/* "0" or the error's name, for messages. */
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static atomic_int handled;

static void on_signal(int sig)
{
	(void)sig;
	atomic_fetch_add(&handled, 1);
}

/* Catches SIGUSR1 with on_signal(), without SA_RESTART (setup). */
static void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction(SIGUSR1) failed: %s", pc_errname(errno));
}

/* Sends SIGUSR1 to a job's thread and waits for the handler to have run. */
static void signal_job(struct job *j)
{
	int before = atomic_load(&handled);

	NEED(pthread_kill(j->thread, SIGUSR1) == 0, "pthread_kill failed");
	NEED(wait_flag_above(&handled, before, 10000), "the SIGUSR1 handler did not run within 10 s");
}

static int do_rdlock(void *l) { return pthread_rwlock_rdlock(l); }
static int do_tryrdlock(void *l) { return pthread_rwlock_tryrdlock(l); }

/* trywrlock in another thread; a lock it gets is released again. */
static int trywr_then_unlock(void *l)
{
	int r = pthread_rwlock_trywrlock(l);

	if (r == 0)
		(void)pthread_rwlock_unlock(l);
	return r;
}

/* A writer: another thread that holds the write lock until told to let go. */
struct writer {
	pthread_rwlock_t *l;
	atomic_int locked, release;
	struct job job;
};

static int writer_main(void *p)
{
	struct writer *w = p;
	int r = pthread_rwlock_wrlock(w->l);

	if (r != 0) {
		atomic_store(&w->locked, 2);
		return r;
	}
	atomic_store(&w->locked, 1);
	while (atomic_load(&w->release) == 0)
		sleep_ms(5);
	return pthread_rwlock_unlock(w->l);
}

static void writer_start(struct writer *w, pthread_rwlock_t *l)
{
	w->l = l;
	atomic_store(&w->locked, 0);
	atomic_store(&w->release, 0);
	job_start(&w->job, writer_main, w);
	NEED(wait_flag(&w->locked, 10000) && atomic_load(&w->locked) == 1, "the writer could not take the write lock");
}

static void writer_end(struct writer *w)
{
	atomic_store(&w->release, 1);
	NEED(job_finish(&w->job, "the writer") == 0, "the writer's unlock failed");
}

int main(void)
{
	static pthread_rwlock_t l;
	struct writer w;
	struct job t;

	pc_begin("XSH.pthread_rwlock_rdlock.002");
	NEED(pthread_rwlock_init(&l, NULL) == 0, "pthread_rwlock_init failed");
	writer_start(&w, &l);
	pc_test();
	job_start(&t, do_tryrdlock, &l);
	pc_check(wait_flag(&t.done, 10000), "pthread_rwlock_tryrdlock() with a writer holding the lock had not returned after 10 s");
	pc_check(t.result == EBUSY, "pthread_rwlock_tryrdlock() with a writer holding the lock returned %s, expected EBUSY",
	    rn(t.result));
	NEED(pthread_join(t.thread, NULL) == 0, "pthread_join failed");
	writer_end(&w);
	pc_pass();
}
