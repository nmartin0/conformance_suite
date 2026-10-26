/*
 * init_routine runs once per once_control.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_once.001
 * MUTANT    pthread_once.always_calls
 * MUTANT    pthread_once.one_control
 * MUTANT    pthread_once.returns_one
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

static pthread_once_t o1 = PTHREAD_ONCE_INIT, o2 = PTHREAD_ONCE_INIT;
static int n1, n2, thread_rets = -1;

static void init1(void)
{
	(void)pthread_mutex_lock(&mx);
	n1++;
	(void)pthread_mutex_unlock(&mx);
}

static void init2(void)
{
	(void)pthread_mutex_lock(&mx);
	n2++;
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	int a = pthread_once(&o1, init1), b = pthread_once(&o2, init2);

	(void)arg;
	post(&thread_rets, a != 0 ? a : b);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r[3], c, i;

	pc_begin("XSH.pthread_once.001");
	pc_test();
	r[0] = pthread_once(&o1, init1);
	r[1] = pthread_once(&o1, init1);
	c = pthread_create(&t, NULL, start, NULL);
	if (c != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(c));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	r[2] = pthread_once(&o2, init2);
	for (i = 0; i < 3; i++)
		pc_check(r[i] == 0, "pthread_once call %d returned %s, expected 0", i + 1, pc_errname(r[i]));
	pc_check(peek(&thread_rets) == 0, "pthread_once in the other thread returned %s", pc_errname(peek(&thread_rets)));
	pc_check(peek(&n1) == 1, "the first init_routine ran %d times, expected once", peek(&n1));
	pc_check(peek(&n2) == 1, "the second once_control's init_routine ran %d times, expected once", peek(&n2));
	pc_pass();
}
