/*
 * Returning from the start routine acts as pthread_exit() with the
 * returned value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.003
 * MUTANT    pthread_create.drops_retval
 * MUTANT    pthread_create.return_skips_dtors
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

static pthread_key_t key;
static int key_obj, ret_obj;
static int dtor_calls, set_failed;
static void *dtor_arg;

static void dtor(void *v)
{
	(void)pthread_mutex_lock(&mx);
	dtor_calls++;
	dtor_arg = v;
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(key, &key_obj) != 0)
		post(&set_failed, 1);
	return &ret_obj;
}

int main(void)
{
	pthread_t t;
	void *v = NULL, *a;
	int r, n;

	pc_begin("XSH.pthread_create.003");
	r = pthread_key_create(&key, dtor);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	r = pthread_join(t, &v);
	if (r != 0)
		pc_unresolved("pthread_join failed: %s", pc_errname(r));
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	pc_check(v == &ret_obj, "pthread_join gave %p, expected the returned %p", v, (void *)&ret_obj);
	(void)pthread_mutex_lock(&mx);
	n = dtor_calls;
	a = dtor_arg;
	(void)pthread_mutex_unlock(&mx);
	pc_check(n == 1 && a == &key_obj, "the key's destructor ran %d times (last argument %p), "
		 "expected once with %p as at pthread_exit()", n, a, (void *)&key_obj);
	pc_pass();
}
