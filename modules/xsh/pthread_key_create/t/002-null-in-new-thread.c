/*
 * A new thread starts with NULL for every key.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_create.002
 * MUTANT    pthread_key_create.inherit_values
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
static int x_obj, seen = -1;

static void *start(void *arg)
{
	(void)arg;
	post(&seen, pthread_getspecific(key) == NULL);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_key_create.002");
	r = pthread_key_create(&key, NULL);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_check(pthread_setspecific(key, &x_obj) == 0, "pthread_setspecific failed");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	pc_check(peek(&seen) == 1, "a new thread started with its creator's value for a key, not NULL");
	pc_pass();
}
