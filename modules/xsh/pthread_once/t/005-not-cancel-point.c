/*
 * pthread_once() is not a cancellation point.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_once.005
 * MUTANT    pthread_once.testcancel
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

/* Sets *p to v and wakes every waiter. */
static void post(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	*p = v;
	(void)pthread_cond_broadcast(&cv);
	(void)pthread_mutex_unlock(&mx);
}

/* Waits until *p is v. */
static void await(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	while (*p != v)
		(void)pthread_cond_wait(&cv, &mx);
	(void)pthread_mutex_unlock(&mx);
}

/* The value of *p, read under the lock. */
static int peek(int *p)
{
	int v;

	(void)pthread_mutex_lock(&mx);
	v = *p;
	(void)pthread_mutex_unlock(&mx);
	return v;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static pthread_once_t once = PTHREAD_ONCE_INIT;
static int ready, requested, calls, after_once, after_test;

static void init(void)
{
	(void)pthread_mutex_lock(&mx);	/* not a cancellation point */
	calls++;
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	int old;

	(void)arg;
	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
	post(&ready, 1);
	await(&requested, 1);	/* the request stays pending while disabled */
	(void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
	(void)pthread_once(&once, init);
	post(&after_once, 1);
	pthread_testcancel();
	post(&after_test, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_once.005");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&ready, 1);
	if (pthread_cancel(t) != 0)
		pc_unresolved("pthread_cancel failed");
	post(&requested, 1);
	if (pthread_join(t, &v) != 0)
		pc_unresolved("pthread_join failed");
	pc_check(peek(&calls) == 1, "init_routine ran %d times, expected once", peek(&calls));
	pc_check(peek(&after_once) == 1, "the pending cancellation was acted on in pthread_once()");
	if (v != PTHREAD_CANCELED || peek(&after_test))
		pc_unresolved("pthread_testcancel() did not act on the pending request");
	pc_pass();
}
