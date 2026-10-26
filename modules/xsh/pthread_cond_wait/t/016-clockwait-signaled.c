/*
 * pthread_cond_clockwait() ends when signaled, returning 0 with the mutex
 * owned by the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_wait.016
 * MUTANT    pthread_cond_wait.clockwait_ignores_signal
 * MUTANT    pthread_cond_wait.returns_unlocked
 * VARIANT   pthread_cond_wait.spurious
 * VARIANT   pthread_cond_signal.wakes_all
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
static int go, waiting, which;	/* under m */
static int r_wait, r_try, r_unlock;

static void *other(void *arg)
{
	(void)arg;
	r_try = pthread_mutex_trylock(&m);
	if (r_try == 0)
		(void)pthread_mutex_unlock(&m);
	return NULL;
}

static void *waiter(void *arg)
{
	clockid_t k = which ? CLOCK_MONOTONIC : CLOCK_REALTIME;
	struct timespec far = ts_after(k, 600000);
	pthread_t o;
	int r = 0;

	(void)arg;
	(void)pthread_mutex_lock(&m);
	waiting = 1;
	while (!go && r == 0)
		r = pthread_cond_clockwait(&c, &m, k, &far);
	r_wait = r;
	r_try = -1;
	if (pthread_create(&o, NULL, other, NULL) == 0)
		(void)pthread_join(o, NULL);
	r_unlock = pthread_mutex_unlock(&m);
	flag_set(1, 1);
	return NULL;
}

int main(void)
{
	static const char *name[2] = { "CLOCK_REALTIME", "CLOCK_MONOTONIC" };
	pthread_t t;
	int i, seen;

	pc_begin("XSH.pthread_cond_wait.016");
	errorcheck_mutex(&m);
	pc_test();
	for (i = 0; i < 2; i++) {
		flag_set(1, 0);
		go = waiting = 0;
		which = i;
		t = start(waiter, NULL);
		do {
			NEED(pthread_mutex_lock(&m) == 0, "pthread_mutex_lock failed");
			seen = waiting;
			if (seen) {
				go = 1;
				pc_check(pthread_cond_signal(&c) == 0, "pthread_cond_signal failed");
			}
			(void)pthread_mutex_unlock(&m);
			if (!seen)
				sleep_ms(1);
		} while (!seen);
		pc_check(wait_flag(1, 1, 10000), "pthread_cond_clockwait(%s) did not return within 10 s of the signal", name[i]);
		(void)pthread_join(t, NULL);
		pc_check(r_wait == 0, "pthread_cond_clockwait(%s) returned %s", name[i], pc_errname(r_wait));
		NEED(r_try != -1, "pthread_create failed");
		pc_check(r_try != 0, "after pthread_cond_clockwait(%s) returned 0 another thread could lock the mutex", name[i]);
		pc_check(r_unlock == 0, "after pthread_cond_clockwait(%s) returned 0 the caller did not own the mutex (unlock: %s)",
			 name[i], pc_errname(r_unlock));
	}
	pc_pass();
}
