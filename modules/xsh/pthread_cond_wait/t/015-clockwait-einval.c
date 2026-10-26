/*
 * pthread_cond_clockwait() fails with EINVAL for a tv_nsec out of range
 * and for a clock that does not exist, leaving the mutex locked.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_wait.015
 * MUTANT    pthread_cond_wait.clockwait_no_einval
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
static clockid_t bad_clock;
static int r_wait[5], r_unlock;

static void *waiter(void *arg)
{
	static const long nsec[2] = { -1, 1000000000L };
	static const clockid_t clk[2] = { CLOCK_REALTIME, CLOCK_MONOTONIC };
	struct timespec abstime;
	int i;

	(void)arg;
	(void)pthread_mutex_lock(&m);
	for (i = 0; i < 4; i++) {
		/* 3 s ahead: the time has not passed, so EINVAL is the only error. */
		abstime = ts_after(clk[i / 2], 3000);
		abstime.tv_nsec = nsec[i % 2];
		r_wait[i] = pthread_cond_clockwait(&c, &m, clk[i / 2], &abstime);
	}
	abstime = ts_after(CLOCK_REALTIME, 3000);
	r_wait[4] = pthread_cond_clockwait(&c, &m, bad_clock, &abstime);
	r_unlock = pthread_mutex_unlock(&m);
	flag_set(1, 1);
	return NULL;
}

int main(void)
{
	static const char *what[5] = {
		"CLOCK_REALTIME and tv_nsec -1", "CLOCK_REALTIME and tv_nsec 1000000000",
		"CLOCK_MONOTONIC and tv_nsec -1", "CLOCK_MONOTONIC and tv_nsec 1000000000",
		"a clock that clock_gettime() rejects",
	};
	static const long candidates[] = { 0x7ffffff0L, 0x12345678L, 4096L, 1000L };
	struct timespec ts;
	unsigned k;
	pthread_t t;
	int i;

	pc_begin("XSH.pthread_cond_wait.015");
	errorcheck_mutex(&m);
	/* A clock ID no clock has: clock_gettime() fails with EINVAL for it. */
	for (k = 0; k < sizeof candidates / sizeof candidates[0]; k++) {
		bad_clock = (clockid_t)candidates[k];
		if (clock_gettime(bad_clock, &ts) == -1 && errno == EINVAL)
			break;
	}
	NEED(k < sizeof candidates / sizeof candidates[0], "no candidate clock ID is rejected by clock_gettime()");
	pc_test();
	t = start(waiter, NULL);
	pc_check(wait_flag(1, 1, 20000), "pthread_cond_clockwait() with bad arguments did not return within 20 s");
	(void)pthread_join(t, NULL);
	for (i = 0; i < 5; i++)
		pc_check(r_wait[i] == EINVAL, "pthread_cond_clockwait() with %s returned %s, not EINVAL", what[i],
			 r_wait[i] == 0 ? "0" : pc_errname(r_wait[i]));
	pc_check(r_unlock == 0, "after the EINVAL returns the caller did not own the mutex (unlock: %s)", pc_errname(r_unlock));
	pc_pass();
}
