/*
 * pthread_join() waits for a running thread to terminate.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_join.001
 * MUTANT    pthread_join.no_wait
 * VARIANT   pthread_create.runs_late
 * VARIANT   pthread_create.runs_first
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

static int ret_obj, done;

static void *start(void *arg)
{
	(void)arg;
	sleep_ms(300);
	post(&done, 1);
	return &ret_obj;
}

int main(void)
{
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_join.001");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	r = pthread_join(t, &v);
	pc_check(r == 0, "pthread_join returned %s, expected 0", pc_errname(r));
	pc_check(peek(&done) == 1, "pthread_join returned before the thread had finished");
	pc_check(v == &ret_obj, "pthread_join gave %p, expected %p", v, (void *)&ret_obj);
	pc_pass();
}
