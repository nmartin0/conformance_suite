/*
 * pthread_rwlock_wrlock() waits for writers and readers to release.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_wrlock.003
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

/* "0" or the error's name, for messages. */
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

/* Takes the write lock and lets it go again in the same thread. */
static int do_wrlock(void *l)
{
	int r = pthread_rwlock_wrlock(l);

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
	struct job j;
	int r, k;

	pc_begin("XSH.pthread_rwlock_wrlock.003");
	NEED(pthread_rwlock_init(&l, NULL) == 0, "pthread_rwlock_init failed");
	pc_test();
	for (k = 0; k < 2; k++) {
		if (k == 0) {
			writer_start(&w, &l);
		} else {
			NEED(pthread_rwlock_rdlock(&l) == 0, "pthread_rwlock_rdlock failed");
		}
		job_start(&j, do_wrlock, &l);
		sleep_ms(300);
		pc_check(!atomic_load(&j.done), "pthread_rwlock_wrlock() returned %s while %s held the lock", rn(j.result),
		    k == 0 ? "a writer" : "a reader");
		if (k == 0) {
			writer_end(&w);
		} else {
			NEED(pthread_rwlock_unlock(&l) == 0, "pthread_rwlock_unlock failed");
		}
		r = job_finish(&j, "pthread_rwlock_wrlock() after the release");
		pc_check(r == 0, "pthread_rwlock_wrlock() returned %s after the release", rn(r));
	}
	pc_pass();
}
