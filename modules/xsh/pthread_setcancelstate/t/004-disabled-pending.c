/*
 * With cancelability disabled a request is held pending: setting the
 * type to ASYNCHRONOUS has no effect, nor do cancellation points; after
 * enabling (deferred) the request is acted on at the next cancellation
 * point, not in pthread_setcancelstate().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_setcancelstate.004
 * MUTANT    pthread_setcancelstate.disable_ignored
 * MUTANT    pthread_setcancelstate.acts_async
 * VARIANT   pthread_setcancelstate.async_deferred
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

/* Setup steps after pc_test() that the assertion does not cover. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * Flags shared between threads, each read and written under flag_lock (a
 * mutex the test never waits on, so that no planted fault touches it).
 */
static pthread_mutex_t flag_lock = PTHREAD_MUTEX_INITIALIZER;
static int flags[16];

static void flag_set(int i, int v)
{
	(void)pthread_mutex_lock(&flag_lock);
	flags[i] = v;
	(void)pthread_mutex_unlock(&flag_lock);
}

static int flag_get(int i)
{
	int v;

	(void)pthread_mutex_lock(&flag_lock);
	v = flags[i];
	(void)pthread_mutex_unlock(&flag_lock);
	return v;
}

static void flag_add(int i, int d)
{
	(void)pthread_mutex_lock(&flag_lock);
	flags[i] += d;
	(void)pthread_mutex_unlock(&flag_lock);
}

static long now_ms(void)
{
	struct timespec ts;

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for flag i to be at least v: 1 if it is. */
static int wait_flag(int i, int v, long ms)
{
	long end = now_ms() + ms;

	while (flag_get(i) < v) {
		if (now_ms() >= end)
			return 0;
		sleep_ms(1);
	}
	return 1;
}

/* The time on clock c, ms milliseconds from now (ms may be negative). */
static struct timespec ts_after(clockid_t c, long ms)
{
	struct timespec ts;

	(void)clock_gettime(c, &ts);
	ts.tv_sec += ms / 1000;
	ts.tv_nsec += (ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L) {
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	} else if (ts.tv_nsec < 0) {
		ts.tv_sec--;
		ts.tv_nsec += 1000000000L;
	}
	return ts;
}

/* 1 if a is not earlier than b. */
static int ts_ge(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
}

/*
 * Joins t through a joiner thread, waiting at most ms milliseconds, so
 * that a thread that never ends gives a verdict instead of a hang: 1 and
 * the thread's status in *ret if it was joined in time, 0 if not.
 */
static pthread_t join_target;
static void *join_status;

static void *joiner(void *arg)
{
	(void)arg;
	if (pthread_join(join_target, &join_status) == 0)
		flag_set(15, 1);
	else
		flag_set(15, 2);
	return NULL;
}

static int join_within(pthread_t t, long ms, void **ret)
{
	pthread_t j;

	flag_set(15, 0);
	join_target = t;
	if (pthread_create(&j, NULL, joiner, NULL) != 0)
		pc_unresolved("pthread_create (joiner) failed");
	if (!wait_flag(15, 1, ms))
		return 0;
	(void)pthread_join(j, NULL);
	if (flag_get(15) != 1)
		pc_unresolved("pthread_join failed");
	if (ret != NULL)
		*ret = join_status;
	return 1;
}

/* Initializes *m as a PTHREAD_MUTEX_ERRORCHECK mutex. */
static void errorcheck_mutex(pthread_mutex_t *m)
{
	pthread_mutexattr_t a;

	if (pthread_mutexattr_init(&a) != 0 || pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK) != 0 ||
	    pthread_mutex_init(m, &a) != 0)
		pc_unresolved("cannot make an error-checking mutex");
	(void)pthread_mutexattr_destroy(&a);
}

/* Starts fn(arg) in a new thread with default attributes. */
static pthread_t start(void *(*fn)(void *), void *arg)
{
	pthread_t t;

	if (pthread_create(&t, NULL, fn, arg) != 0)
		pc_unresolved("pthread_create failed");
	return t;
}

static pthread_mutex_t gate = PTHREAD_MUTEX_INITIALIZER;
static int fds[2];

static void *target(void *arg)
{
	struct timespec ts = { 0, 10000000L };
	char b;
	int old;
	long end;

	(void)arg;
	if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old) != 0 ||
	    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old) != 0) {
		flag_set(0, -1);
		return NULL;
	}
	flag_set(0, 1);
	/* Disabled and asynchronous: wait for the request (flag 1), then 200 ms more. */
	/*
	 * Wait for the request: main holds gate until pthread_cancel() has
	 * returned. Only the gate is taken here (never flag_lock), so a
	 * system that ended the thread here could not leave a lock held
	 * that the main thread needs.
	 */
	while (pthread_mutex_trylock(&gate) != 0)
		(void)sched_yield();	/* not a cancellation point */
	(void)pthread_mutex_unlock(&gate);
	end = now_ms() + 200;
	while (now_ms() < end)
		;
	flag_set(2, 1);		/* survived the request while disabled */
	(void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
	(void)nanosleep(&ts, NULL);	/* cancellation points, all with cancelability disabled */
	(void)read(fds[0], &b, 1);	/* a byte is there: does not block */
	flag_set(3, 1);
	(void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
	flag_set(4, 1);		/* enabling is not a cancellation point */
	pthread_testcancel();
	flag_set(5, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *st;
	int r;

	pc_begin("XSH.pthread_setcancelstate.004");
	NEED(pipe(fds) == 0 && write(fds[1], "x", 1) == 1, "pipe failed");
	NEED(pthread_mutex_lock(&gate) == 0, "pthread_mutex_lock failed");
	pc_test();
	t = start(target, NULL);
	NEED(wait_flag(0, 1, 10000) && flag_get(0) == 1, "the thread could not disable cancellation and set the type");
	r = pthread_cancel(t);
	NEED(r == 0, "pthread_cancel() returned %s", pc_errname(r));
	(void)pthread_mutex_unlock(&gate);
	pc_check(join_within(t, 15000, &st), "the thread did not terminate within 15 s");
	pc_check(flag_get(2) == 1, "a thread with cancelability disabled (type ASYNCHRONOUS) was canceled while it ran");
	pc_check(flag_get(3) == 1, "a thread with cancelability disabled was canceled at nanosleep() or read()");
	pc_check(flag_get(4) == 1, "pthread_setcancelstate(PTHREAD_CANCEL_ENABLE) acted on the pending request itself");
	pc_check(flag_get(5) == 0 && st == PTHREAD_CANCELED,
		 "after cancelability was enabled the pending request was not acted on at pthread_testcancel()");
	pc_pass();
}
