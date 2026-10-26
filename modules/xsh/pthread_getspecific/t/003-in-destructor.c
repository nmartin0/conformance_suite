/*
 * pthread_getspecific() of the key being destroyed, in its destructor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getspecific.003
 * MUTANT    pthread_getspecific.dtor_sees_value
 * MUTANT    pthread_getspecific.dtor_set_lost
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
static int a_obj, c_obj, calls, set_failed;
static void *got_on_entry = &a_obj, *got_after_set;

static void dtor(void *v)
{
	void *g = pthread_getspecific(key), *h = NULL;
	int n;

	(void)v;
	(void)pthread_mutex_lock(&mx);
	n = ++calls;
	(void)pthread_mutex_unlock(&mx);
	if (n != 1)
		return;
	if (pthread_setspecific(key, &c_obj) != 0)
		post(&set_failed, 1);
	h = pthread_getspecific(key);
	(void)pthread_mutex_lock(&mx);
	got_on_entry = g;
	got_after_set = h;
	(void)pthread_mutex_unlock(&mx);
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
	void *g, *h;
	int r;

	pc_begin("XSH.pthread_getspecific.003");
	r = pthread_key_create(&key, dtor);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed");
	if (peek(&calls) == 0)
		pc_unresolved("the destructor was not called");
	(void)pthread_mutex_lock(&mx);
	g = got_on_entry;
	h = got_after_set;
	(void)pthread_mutex_unlock(&mx);
	pc_check(g == NULL, "in the destructor, pthread_getspecific of its key returned %p, expected NULL", g);
	pc_check(h == &c_obj, "after pthread_setspecific(%p) in the destructor, pthread_getspecific returned %p",
		 (void *)&c_obj, h);
	pc_pass();
}
