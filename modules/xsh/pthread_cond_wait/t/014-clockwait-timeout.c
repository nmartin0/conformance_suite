/*
 * pthread_cond_clockwait() times out against the clock it is given
 * (CLOCK_REALTIME and CLOCK_MONOTONIC), not early, holding the mutex.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_wait.014
 * MUTANT    pthread_cond_wait.clockwait_early
 * MUTANT    pthread_cond_wait.clockwait_swapped
 * VARIANT   pthread_cond_wait.spurious
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

static pthread_mutex_t m;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static const clockid_t clk[2] = { CLOCK_REALTIME, CLOCK_MONOTONIC };
static int r_last[4], early[4], r_unlock[4];

static int wait_until(clockid_t k, struct timespec abstime)
{
	struct timespec stop = abstime;
	int r;

	stop.tv_sec += 10;
	do {
		r = pthread_cond_clockwait(&c, &m, k, &abstime);
	} while (r == 0 && !ts_ge(ts_after(k, 0), stop));
	return r;
}

static void *waiter(void *arg)
{
	struct timespec abstime;
	int i;

	(void)arg;
	for (i = 0; i < 4; i++) {
		abstime = ts_after(clk[i / 2], i % 2 == 0 ? -1000 : 300);
		if (abstime.tv_sec < 0)
			abstime.tv_sec = abstime.tv_nsec = 0;
		(void)pthread_mutex_lock(&m);
		r_last[i] = wait_until(clk[i / 2], abstime);
		early[i] = !ts_ge(ts_after(clk[i / 2], 0), abstime);
		r_unlock[i] = pthread_mutex_unlock(&m);
	}
	flag_set(1, 1);
	return NULL;
}

int main(void)
{
	static const char *what[4] = {
		"a CLOCK_REALTIME abstime in the past", "a CLOCK_REALTIME abstime 300 ms ahead",
		"a CLOCK_MONOTONIC abstime in the past", "a CLOCK_MONOTONIC abstime 300 ms ahead",
	};
	pthread_t t;
	int i;

	pc_begin("XSH.pthread_cond_wait.014");
	errorcheck_mutex(&m);
	pc_test();
	t = start(waiter, NULL);
	pc_check(wait_flag(1, 1, 50000), "pthread_cond_clockwait() with no signal did not return ETIMEDOUT within 10 s of abstime");
	(void)pthread_join(t, NULL);
	for (i = 0; i < 4; i++) {
		pc_check(r_last[i] == ETIMEDOUT, "with %s pthread_cond_clockwait() returned %s, not ETIMEDOUT", what[i],
			 r_last[i] == 0 ? "only 0 for 10 s past abstime" : pc_errname(r_last[i]));
		pc_check(!early[i], "with %s pthread_cond_clockwait() returned ETIMEDOUT before the clock reached abstime", what[i]);
		pc_check(r_unlock[i] == 0, "after ETIMEDOUT (%s) the caller did not own the mutex (unlock: %s)", what[i],
			 pc_errname(r_unlock[i]));
	}
	pc_pass();
}
