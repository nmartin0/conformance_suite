/*
 * A wait on a mutex the caller does not own fails with EPERM (error
 * checking and robust mutexes) and leaves the mutex as it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_wait.007
 * MUTANT    pthread_cond_wait.eperm_missing
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

static pthread_mutex_t ec, rb;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static int res[8];

/* The holder locks ec, reports, and unlocks it once flag 2 is set. */
static void *holder(void *arg)
{
	(void)arg;
	if (pthread_mutex_lock(&ec) != 0) {
		flag_set(0, -1);
		return NULL;
	}
	flag_set(0, 1);
	(void)wait_flag(2, 1, 20000);
	res[7] = pthread_mutex_unlock(&ec);
	flag_set(3, 1);
	return NULL;
}

static int timed(pthread_mutex_t *mx)
{
	struct timespec abstime = ts_after(CLOCK_REALTIME, 3000);

	return pthread_cond_timedwait(&c, mx, &abstime);
}

/* Each call may block forever on a faulty system: they run in this thread. */
static void *caller(void *arg)
{
	(void)arg;
	flag_set(4, 0);
	res[0] = pthread_cond_wait(&c, &ec);		/* unlocked */
	flag_set(4, 1);
	res[1] = timed(&ec);
	res[2] = pthread_mutex_trylock(&ec);		/* still unlocked? */
	if (res[2] == 0)
		(void)pthread_mutex_unlock(&ec);
	flag_set(4, 4);
	res[4] = timed(&rb);				/* robust, unlocked */
	flag_set(4, 3);
	res[3] = pthread_cond_wait(&c, &rb);
	flag_set(1, 1);
	return NULL;
}

/* Calls on ec while the holder thread owns it. */
static void *caller2(void *arg)
{
	(void)arg;
	flag_set(4, 6);
	res[6] = timed(&ec);
	flag_set(4, 5);
	res[5] = pthread_cond_wait(&c, &ec);
	flag_set(1, 2);
	return NULL;
}

int main(void)
{
	static const char *what[] = {
		"pthread_cond_wait() on an unlocked error-checking mutex",
		"pthread_cond_timedwait() on an unlocked error-checking mutex",
		"", "pthread_cond_wait() on an unlocked robust mutex",
		"pthread_cond_timedwait() on an unlocked robust mutex",
		"pthread_cond_wait() on an error-checking mutex locked by another thread",
		"pthread_cond_timedwait() on an error-checking mutex locked by another thread",
	};
	static const int idx[] = { 0, 1, 3, 4, 5, 6 };
	pthread_mutexattr_t a;
	pthread_t t, h;
	unsigned i;

	pc_begin("XSH.pthread_cond_wait.007");
	errorcheck_mutex(&ec);
	NEED(pthread_mutexattr_init(&a) == 0 && pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST) == 0 &&
	     pthread_mutex_init(&rb, &a) == 0, "cannot make a robust mutex");
	pc_test();
	t = start(caller, NULL);
	pc_check(wait_flag(1, 1, 20000), "%s did not return within 20 s", what[flag_get(4)]);
	(void)pthread_join(t, NULL);
	h = start(holder, NULL);
	NEED(wait_flag(0, 1, 10000), "the holder thread could not lock the mutex");
	t = start(caller2, NULL);
	pc_check(wait_flag(1, 2, 20000), "%s did not return within 20 s", what[flag_get(4)]);
	(void)pthread_join(t, NULL);
	flag_set(2, 1);
	NEED(wait_flag(3, 1, 10000), "the holder thread did not finish");
	(void)pthread_join(h, NULL);
	for (i = 0; i < sizeof idx / sizeof idx[0]; i++)
		pc_check(res[idx[i]] == EPERM, "%s returned %s, not EPERM", what[idx[i]],
			 res[idx[i]] == 0 ? "0" : pc_errname(res[idx[i]]));
	pc_check(res[2] == 0, "after the EPERM returns the unlocked mutex could not be locked (%s)", pc_errname(res[2]));
	pc_check(res[7] == 0, "after the EPERM returns the holder no longer owned its mutex (unlock: %s)",
		 pc_errname(res[7]));
	pc_pass();
}
