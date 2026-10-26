/*
 * pthread_create() with a null attr runs start_routine(arg) in a joinable
 * thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.001
 * MUTANT    pthread_create.arg_null
 * MUTANT    pthread_create.returns_minus1
 * MUTANT    pthread_create.calls_twice
 * VARIANT   pthread_create.runs_late
 * VARIANT   pthread_create.runs_first
 * VARIANT   pthread_create.late_store
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

static int arg_obj, ret_obj;
static int calls;
static void *seen;

static void *start(void *arg)
{
	(void)pthread_mutex_lock(&mx);
	calls++;
	seen = arg;
	(void)pthread_mutex_unlock(&mx);
	return &ret_obj;
}

int main(void)
{
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_create.001");
	pc_test();
	r = pthread_create(&t, NULL, start, &arg_obj);
	pc_check(r == 0, "pthread_create returned %d (%s), expected 0", r, pc_errname(r));
	r = pthread_join(t, &v);
	pc_check(r == 0, "pthread_join of a thread created with a null attr returned %s: "
		 "the default attributes make a joinable thread", pc_errname(r));
	pc_check(v == &ret_obj, "pthread_join gave %p, expected the start routine's %p", v, (void *)&ret_obj);
	pc_check(peek(&calls) == 1, "start_routine ran %d times, expected once", peek(&calls));
	(void)pthread_mutex_lock(&mx);
	v = seen;
	(void)pthread_mutex_unlock(&mx);
	pc_check(v == &arg_obj, "start_routine got %p, expected arg %p", v, (void *)&arg_obj);
	pc_pass();
}
