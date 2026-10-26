/*
 * Destructors run at thread exit for non-NULL values only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_create.003
 * MUTANT    pthread_key_create.no_dtor
 * MUTANT    pthread_key_create.dtor_arg_null
 * MUTANT    pthread_key_create.dtor_on_null
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

static pthread_key_t k1, k2, k3;
static int a_obj, b_obj, n1, n2, n3, set_failed;
static void *arg1;

static void d1(void *v)
{
	(void)pthread_mutex_lock(&mx);
	n1++;
	arg1 = v;
	(void)pthread_mutex_unlock(&mx);
}

static void d2(void *v)
{
	(void)v;
	(void)pthread_mutex_lock(&mx);
	n2++;
	(void)pthread_mutex_unlock(&mx);
}

static void d3(void *v)
{
	(void)v;
	(void)pthread_mutex_lock(&mx);
	n3++;
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	(void)arg;
	/* k1 non-NULL; k2 never set; k3 set and then set back to NULL */
	if (pthread_setspecific(k1, &a_obj) != 0 || pthread_setspecific(k3, &b_obj) != 0
	    || pthread_setspecific(k3, NULL) != 0)
		post(&set_failed, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_key_create.003");
	r = pthread_key_create(&k1, d1);
	if (r == 0)
		r = pthread_key_create(&k2, d2);
	if (r == 0)
		r = pthread_key_create(&k3, d3);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	pc_check(peek(&n1) == 1, "the destructor of a key with a non-NULL value ran %d times, expected once", peek(&n1));
	(void)pthread_mutex_lock(&mx);
	r = arg1 == &a_obj;
	(void)pthread_mutex_unlock(&mx);
	pc_check(r, "the destructor did not get the thread's value");
	pc_check(peek(&n2) == 0, "the destructor of a key never set in the thread ran");
	pc_check(peek(&n3) == 0, "the destructor of a key set back to NULL in the thread ran");
	pc_pass();
}
