/*
 * Changing the attributes object after pthread_create() does not change
 * the thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.011
 * MUTANT    pthread_create.attr_read_late
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

static int ret_obj, running;

static void *start(void *arg)
{
	(void)arg;
	post(&running, 1);
	return &ret_obj;
}

int main(void)
{
	pthread_attr_t a;
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_create.011");
	pc_check(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_check(pthread_attr_setdetachstate(&a, PTHREAD_CREATE_JOINABLE) == 0, "pthread_attr_setdetachstate failed");
	pc_test();
	r = pthread_create(&t, &a, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED) != 0)
		pc_unresolved("pthread_attr_setdetachstate(PTHREAD_CREATE_DETACHED) failed");
	await(&running, 1);
	sleep_ms(200);	/* time for a late look at the attributes */
	r = pthread_join(t, &v);
	pc_check(r == 0, "pthread_join returned %s: the thread did not stay joinable", pc_errname(r));
	pc_check(v == &ret_obj, "pthread_join gave %p, expected %p", v, (void *)&ret_obj);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
