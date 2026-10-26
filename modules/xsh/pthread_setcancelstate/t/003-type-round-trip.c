/*
 * pthread_setcanceltype() sets the type and stores the one it replaced.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_setcancelstate.003
 * MUTANT    pthread_setcancelstate.type_old_wrong
 * MUTANT    pthread_setcancelstate.async_ignored
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

static int r[4], old[4];

static void *body(void *arg)
{
	static const int set[4] = { PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DEFERRED,
				    PTHREAD_CANCEL_DEFERRED };
	int i;

	(void)arg;
	/*
	 * No request is ever made against this thread, so being
	 * asynchronously cancelable here is harmless; only
	 * pthread_setcanceltype() (async-cancel-safe) is called meanwhile.
	 */
	for (i = 0; i < 4; i++) {
		old[i] = -1;
		r[i] = pthread_setcanceltype(set[i], &old[i]);
	}
	return NULL;
}

static const char *name(int s)
{
	return s == PTHREAD_CANCEL_DEFERRED ? "PTHREAD_CANCEL_DEFERRED"
	       : s == PTHREAD_CANCEL_ASYNCHRONOUS ? "PTHREAD_CANCEL_ASYNCHRONOUS" : "neither value";
}

int main(void)
{
	static const int want[4] = { PTHREAD_CANCEL_DEFERRED, PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_ASYNCHRONOUS,
				     PTHREAD_CANCEL_DEFERRED };
	static const char *call[4] = { "ASYNCHRONOUS (from the default)", "ASYNCHRONOUS again", "DEFERRED",
				       "DEFERRED again" };
	pthread_t t;
	int i;

	pc_begin("XSH.pthread_setcancelstate.003");
	pc_test();
	t = start(body, NULL);
	(void)pthread_join(t, NULL);
	for (i = 0; i < 4; i++) {
		pc_check(r[i] == 0, "pthread_setcanceltype(%s) returned %s", call[i], pc_errname(r[i]));
		pc_check(old[i] == want[i], "pthread_setcanceltype(%s) stored %s as the previous type, not %s", call[i],
			 name(old[i]), name(want[i]));
	}
	pc_pass();
}
