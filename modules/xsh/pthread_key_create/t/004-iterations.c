/*
 * Destructors are called again while values are set again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_create.004
 * MUTANT    pthread_key_create.single_pass
 * MUTANT    pthread_key_create.extra_pass
 * VARIANT   pthread_create.runs_late
 * VARIANT   pthread_key_create.dtors_reversed
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

static pthread_key_t key;
static int a_obj, calls, set_failed;

static void dtor(void *v)
{
	int n;

	(void)v;
	(void)pthread_mutex_lock(&mx);
	n = ++calls;
	(void)pthread_mutex_unlock(&mx);
	if (n < 4 && pthread_setspecific(key, &a_obj) != 0)
		post(&set_failed, 1);
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(key, &a_obj) != 0)
		post(&set_failed, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_key_create.004");
	r = pthread_key_create(&key, dtor);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	pc_check(peek(&calls) == 4, "the destructor ran %d times, expected 4 (it set the value again on calls 1 to 3)",
		 peek(&calls));
	pc_pass();
}
