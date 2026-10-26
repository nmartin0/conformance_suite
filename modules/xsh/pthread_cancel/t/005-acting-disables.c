/*
 * While a cancellation is acted on, the cleanup handler finds the
 * cancelability state PTHREAD_CANCEL_DISABLE and the type
 * PTHREAD_CANCEL_DEFERRED, though the thread had made itself
 * asynchronously cancelable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cancel.005
 * MUTANT    pthread_cancel.noop
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

static volatile int spin_stop;	/* the target's own: volatile keeps the spin a real loop */

static void handler(void *arg)
{
	int s = -1, ty = -1;

	(void)arg;
	/* Setting DISABLE and DEFERRED is allowed in a handler; it reports what was there. */
	flag_set(2, pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &s) == 0 ? 1 : -1);
	flag_set(3, pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &ty) == 0 ? 1 : -1);
	flag_set(4, s == PTHREAD_CANCEL_DISABLE ? 1 : s == PTHREAD_CANCEL_ENABLE ? 2 : 3);
	flag_set(5, ty == PTHREAD_CANCEL_DEFERRED ? 1 : ty == PTHREAD_CANCEL_ASYNCHRONOUS ? 2 : 3);
	flag_set(6, 1);
}

static void *target(void *arg)
{
	long end;
	unsigned long n = 0;
	int old;

	(void)arg;
	pthread_cleanup_push(handler, NULL);
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old) != 0)
		flag_set(0, -1);
	else
		flag_set(0, 1);
	/*
	 * Spin with no function call, for about 3 s after the request (flag
	 * 1). A system that acts on it now, asynchronously, runs the handler
	 * with the type it had to set; one that waits for a cancellation
	 * point gets one below, after the switch back to DEFERRED (only
	 * async-cancel-safe functions are called while ASYNCHRONOUS).
	 */
	end = 0;
	while (!spin_stop) {
		if (++n % 1000000 == 0) {
			(void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
			if (flag_get(1) && end == 0)
				end = now_ms() + 3000;
			if (end != 0 && now_ms() >= end)
				spin_stop = 1;
			else
				(void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
		}
	}
	pthread_testcancel();
	flag_set(7, 1);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *st;
	int r;

	pc_begin("XSH.pthread_cancel.005");
	pc_test();
	t = start(target, NULL);
	NEED(wait_flag(0, 1, 10000) && flag_get(0) == 1, "the thread could not make itself asynchronously cancelable");
	r = pthread_cancel(t);
	NEED(r == 0, "pthread_cancel() returned %s", pc_errname(r));
	flag_set(1, 1);
	pc_check(wait_flag(6, 1, 20000), "the cleanup handler did not run within 20 s");
	pc_check(join_within(t, 10000, &st) && st == PTHREAD_CANCELED, "the thread did not end canceled");
	NEED(flag_get(2) == 1 && flag_get(3) == 1, "pthread_setcancelstate() or pthread_setcanceltype() failed in the handler");
	pc_check(flag_get(4) == 1, "in the cleanup handler the cancelability state was %s, not PTHREAD_CANCEL_DISABLE",
		 flag_get(4) == 2 ? "PTHREAD_CANCEL_ENABLE" : "neither value");
	pc_check(flag_get(5) == 1, "in the cleanup handler the cancelability type was %s, not PTHREAD_CANCEL_DEFERRED",
		 flag_get(5) == 2 ? "PTHREAD_CANCEL_ASYNCHRONOUS" : "neither value");
	pc_pass();
}
