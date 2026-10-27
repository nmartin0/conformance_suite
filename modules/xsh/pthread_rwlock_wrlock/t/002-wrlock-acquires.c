/*
 * pthread_rwlock_wrlock() takes a free lock for writing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_wrlock.002
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

/* tryrdlock in another thread; a lock it gets is released again. */
static int tryrd_then_unlock(void *l)
{
	int r = pthread_rwlock_tryrdlock(l);

	if (r == 0)
		(void)pthread_rwlock_unlock(l);
	return r;
}

int main(void)
{
	static pthread_rwlock_t l;
	int r;

	pc_begin("XSH.pthread_rwlock_wrlock.002");
	NEED(pthread_rwlock_init(&l, NULL) == 0, "pthread_rwlock_init failed");
	pc_test();
	r = pthread_rwlock_wrlock(&l);
	pc_check(r == 0, "pthread_rwlock_wrlock() of a free lock returned %s", rn(r));
	r = in_other_thread(tryrd_then_unlock, &l, "tryrdlock");
	pc_check(r == EBUSY, "with the write lock held, another thread's tryrdlock returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "pthread_rwlock_unlock() returned %s", rn(r));
	r = in_other_thread(tryrd_then_unlock, &l, "tryrdlock");
	pc_check(r == 0, "after the unlock, another thread's tryrdlock returned %s, expected 0", rn(r));
	pc_pass();
}
