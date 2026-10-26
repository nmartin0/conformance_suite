/*
 * pthread_key_delete() of keys with and without values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_delete.001
 * MUTANT    pthread_key_delete.einval_in_use
 * MUTANT    pthread_key_delete.minus1
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
static int a_obj, b_obj, set_done, release, set_failed;

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(key, &b_obj) != 0)
		post(&set_failed, 1);
	post(&set_done, 1);
	await(&release, 1);	/* still live, value still set, at the deletion */
	return NULL;
}

int main(void)
{
	pthread_key_t unused;
	pthread_t t;
	int r, r2;

	pc_begin("XSH.pthread_key_delete.001");
	r = pthread_key_create(&key, NULL);
	if (r == 0)
		r = pthread_key_create(&unused, NULL);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_check(pthread_setspecific(key, &a_obj) == 0, "pthread_setspecific failed");
	r = pthread_create(&t, NULL, start, NULL);
	pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	await(&set_done, 1);
	pc_check(!peek(&set_failed), "pthread_setspecific failed in the thread");
	pc_test();
	r = pthread_key_delete(key);
	r2 = pthread_key_delete(unused);
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(r == 0, "pthread_key_delete of a key with values set returned %s, expected 0", pc_errname(r));
	pc_check(r2 == 0, "pthread_key_delete of a key never set returned %s, expected 0", pc_errname(r2));
	pc_pass();
}
