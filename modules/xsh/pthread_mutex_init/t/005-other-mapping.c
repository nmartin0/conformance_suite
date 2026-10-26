/*
 * A process-shared mutex works through another mapping of its memory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_init.005
 * MUTANT    pthread_mutex_init.pshared_by_address
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

static int do_lock(void *m) { return pthread_mutex_lock(m); }
static int do_trylock(void *m) { return pthread_mutex_trylock(m); }
static int do_unlock(void *m) { return pthread_mutex_unlock(m); }

/*
 * Initializes m with the given type (-1: attributes left at their
 * defaults) and robustness and, if pshared, PTHREAD_PROCESS_SHARED.
 * Returns pthread_mutex_init()'s result; any other step failing is
 * UNRESOLVED.
 */
static int make_mutex(pthread_mutex_t *m, int type, int robust, int pshared)
{
	pthread_mutexattr_t a;
	int r;

	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	if (type != -1)
		NEED(pthread_mutexattr_settype(&a, type) == 0, "pthread_mutexattr_settype(%d) failed", type);
	if (robust)
		NEED(pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST) == 0, "pthread_mutexattr_setrobust failed");
	if (pshared)
		NEED(pthread_mutexattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0, "pthread_mutexattr_setpshared failed");
	r = pthread_mutex_init(m, &a);
	(void)pthread_mutexattr_destroy(&a);
	return r;
}

/* As make_mutex(), where failing is UNRESOLVED. */
static void new_mutex(pthread_mutex_t *m, int type)
{
	int r = make_mutex(m, type, 0, 0);

	NEED(r == 0, "pthread_mutex_init failed: %s", pc_errname(r));
}

/* Checks that another thread's trylock returns want (0 is unlocked again by that thread). */
static int trylock_then_unlock(void *m)
{
	int r = pthread_mutex_trylock(m);

	if (r == 0 || r == EOWNERDEAD)
		(void)pthread_mutex_unlock(m);
	return r;
}

static void other_trylock_is(pthread_mutex_t *m, int want, const char *when)
{
	int r = in_other_thread(trylock_then_unlock, m, "pthread_mutex_trylock()");

	pc_check(r == want, "%s, another thread's pthread_mutex_trylock() returned %s, expected %s",
	    when, r == 0 ? "0" : pc_errname(r), want == 0 ? "0" : pc_errname(want));
}

/*
 * A holder: another thread that locks the mutex (1 in locked once it
 * holds it, 2 if the lock failed) and unlocks it when told to.
 */
struct holder {
	pthread_mutex_t *m;
	atomic_int locked, release;
	struct job job;
};

static int hold_main(void *p)
{
	struct holder *h = p;
	int r = pthread_mutex_lock(h->m);

	if (r != 0) {
		atomic_store(&h->locked, 2);
		return r;
	}
	atomic_store(&h->locked, 1);
	while (atomic_load(&h->release) == 0)
		sleep_ms(5);
	return pthread_mutex_unlock(h->m);
}

static void hold_start(struct holder *h, pthread_mutex_t *m)
{
	h->m = m;
	atomic_store(&h->locked, 0);
	atomic_store(&h->release, 0);
	job_start(&h->job, hold_main, h);
	NEED(wait_flag(&h->locked, 10000) && atomic_load(&h->locked) == 1,
	    "the holder thread could not lock the mutex");
}

/* Lets the holder unlock; returns its pthread_mutex_unlock() result. */
static int hold_end(struct holder *h)
{
	atomic_store(&h->release, 1);
	return job_finish(&h->job, "the holder thread");
}

int main(void)
{
	long pg = sysconf(_SC_PAGESIZE);
	size_t len = pg > 0 ? (size_t)pg : 4096;
	pthread_mutex_t *a, *b;
	struct job w;
	int fd, r;

	pc_begin("XSH.pthread_mutex_init.005");
	fd = open("mutex", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(ftruncate(fd, (off_t)len) == 0, "ftruncate failed: %s", pc_errname(errno));
	a = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	b = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(a != MAP_FAILED && b != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	NEED(a != b, "the two mappings have the same address");
	(void)close(fd);
	r = make_mutex(a, PTHREAD_MUTEX_NORMAL, 0, 1);
	NEED(r == 0, "pthread_mutex_init of a process-shared mutex failed: %s", rn(r));
	pc_test();
	r = pthread_mutex_lock(b);
	pc_check(r == 0, "pthread_mutex_lock() through the second mapping returned %s", rn(r));
	other_trylock_is(a, EBUSY, "with the mutex locked through the second mapping, through the first");
	r = pthread_mutex_unlock(b);
	pc_check(r == 0, "pthread_mutex_unlock() through the second mapping returned %s", rn(r));
	other_trylock_is(a, 0, "after the unlock through the second mapping, through the first");
	r = pthread_mutex_lock(a);
	pc_check(r == 0, "pthread_mutex_lock() through the first mapping returned %s", rn(r));
	job_start(&w, do_lock, b);
	sleep_ms(200);
	pc_check(!atomic_load(&w.done), "a lock through the second mapping returned %s while it was locked through the first",
	    rn(w.result));
	r = pthread_mutex_unlock(a);
	pc_check(r == 0, "pthread_mutex_unlock() through the first mapping returned %s", rn(r));
	r = job_finish(&w, "a lock through the second mapping after the unlock through the first");
	pc_check(r == 0, "the waiting lock through the second mapping returned %s", rn(r));
	pc_pass();
}
