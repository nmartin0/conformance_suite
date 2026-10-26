/*
 * A new key has the value NULL in every live thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_create.001
 * MUTANT    pthread_key_create.creator_value
 * MUTANT    pthread_key_create.others_value
 * MUTANT    pthread_key_create.stale_after_delete
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

static pthread_key_t k0, k1;
static int x_obj, step, k0_before = -1, k1_seen = -1, set_failed;

/* Steps: 1 k0 made, 2 thread set k0, 3 k1 made, 4 thread looked. */
static void *start(void *arg)
{
	(void)arg;
	await(&step, 1);
	(void)pthread_mutex_lock(&mx);
	k0_before = pthread_getspecific(k0) == NULL;
	(void)pthread_mutex_unlock(&mx);
	if (pthread_setspecific(k0, &x_obj) != 0)
		post(&set_failed, 1);
	post(&step, 2);
	await(&step, 3);
	(void)pthread_mutex_lock(&mx);
	k1_seen = pthread_getspecific(k1) == NULL;
	(void)pthread_mutex_unlock(&mx);
	post(&step, 4);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, r1, mine;

	pc_begin("XSH.pthread_key_create.001");
	r = pthread_create(&t, NULL, start, NULL);
	pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_key_create(&k0, NULL);
	pc_check(r == 0, "pthread_key_create returned %s, expected 0", pc_errname(r));
	post(&step, 1);
	await(&step, 2);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the other thread");
	if (pthread_key_delete(k0) != 0)
		pc_unresolved("pthread_key_delete failed");
	r1 = pthread_key_create(&k1, NULL);
	pc_check(r1 == 0, "the second pthread_key_create returned %s, expected 0", pc_errname(r1));
	mine = pthread_getspecific(k1) == NULL;
	post(&step, 3);
	await(&step, 4);
	(void)pthread_join(t, NULL);
	pc_check(k0_before == 1, "a key made while the other thread was live had a non-NULL value there");
	pc_check(mine, "a new key has a non-NULL value in the creating thread");
	pc_check(k1_seen == 1, "a key made after another was deleted has a non-NULL value in a live thread "
		 "that had set the deleted key");
	pc_pass();
}
