/*
 * pthread_once() returns only after init_routine has completed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_once.002
 * MUTANT    pthread_once.no_wait
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

#define N 4

static pthread_once_t once = PTHREAD_ONCE_INIT;
static int calls, done, go, saw_done[N], rets[N];

static void init(void)
{
	(void)pthread_mutex_lock(&mx);
	calls++;
	(void)pthread_mutex_unlock(&mx);
	sleep_ms(300);
	post(&done, 1);
}

static void *start(void *arg)
{
	int i = *(int *)arg, r;

	await(&go, 1);
	r = pthread_once(&once, init);
	(void)pthread_mutex_lock(&mx);
	rets[i] = r;
	saw_done[i] = done;
	(void)pthread_mutex_unlock(&mx);
	return NULL;
}

int main(void)
{
	static int idx[N] = { 0, 1, 2, 3 };
	pthread_t t[N];
	int i, r;

	pc_begin("XSH.pthread_once.002");
	for (i = 0; i < N; i++) {
		r = pthread_create(&t[i], NULL, start, &idx[i]);
		pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	}
	pc_test();
	post(&go, 1);
	for (i = 0; i < N; i++)
		if (pthread_join(t[i], NULL) != 0)
			pc_unresolved("pthread_join failed");
	for (i = 0; i < N; i++) {
		pc_check(rets[i] == 0, "pthread_once in thread %d returned %s", i, pc_errname(rets[i]));
		pc_check(saw_done[i] == 1, "pthread_once in thread %d returned before init_routine had completed", i);
	}
	pc_check(calls == 1, "init_routine ran %d times, expected once", calls);
	pc_pass();
}
