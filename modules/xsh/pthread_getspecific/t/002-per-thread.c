/*
 * Thread-specific values are kept per thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getspecific.002
 * MUTANT    pthread_getspecific.shared
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

#define N 3

static pthread_key_t key;
static int objs[N + 1], set_count, ok[N] = { -1, -1, -1 }, set_failed;

/* Threads 0 and 1 set a value, thread 2 sets none; all read back once every set is done. */
static void *start(void *arg)
{
	int i = *(int *)arg;
	void *v;

	if (i < 2 && pthread_setspecific(key, &objs[i]) != 0)
		post(&set_failed, 1);
	(void)pthread_mutex_lock(&mx);
	set_count++;
	(void)pthread_cond_broadcast(&cv);
	while (set_count < N + 1)
		(void)pthread_cond_wait(&cv, &mx);
	(void)pthread_mutex_unlock(&mx);
	v = pthread_getspecific(key);
	post(&ok[i], i < 2 ? v == &objs[i] : v == NULL);
	return NULL;
}

int main(void)
{
	static int idx[N] = { 0, 1, 2 };
	pthread_t t[N];
	void *mine;
	int i, r;

	pc_begin("XSH.pthread_getspecific.002");
	r = pthread_key_create(&key, NULL);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	for (i = 0; i < N; i++) {
		r = pthread_create(&t[i], NULL, start, &idx[i]);
		if (r != 0)
			pc_unresolved("pthread_create failed: %s", pc_errname(r));
	}
	if (pthread_setspecific(key, &objs[N]) != 0)
		pc_unresolved("pthread_setspecific failed");
	(void)pthread_mutex_lock(&mx);
	set_count++;
	(void)pthread_cond_broadcast(&cv);
	while (set_count < N + 1)
		(void)pthread_cond_wait(&cv, &mx);
	(void)pthread_mutex_unlock(&mx);
	mine = pthread_getspecific(key);
	for (i = 0; i < N; i++)
		(void)pthread_join(t[i], NULL);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in a thread");
	pc_check(mine == &objs[N], "the initial thread reads %p, not the value it set", mine);
	pc_check(peek(&ok[0]) == 1 && peek(&ok[1]) == 1, "a thread did not read back its own value");
	pc_check(peek(&ok[2]) == 1, "a thread that set no value read another thread's, not NULL");
	pc_pass();
}
