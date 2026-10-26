/*
 * pthread_create() stores the new thread's ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.002
 * MUTANT    pthread_create.stores_creator_id
 * MUTANT    pthread_create.stores_last_id
 * VARIANT   pthread_create.late_store
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

static pthread_t self_of[2];
static int ready[2], release;

static void *start(void *arg)
{
	int i = *(int *)arg;

	(void)pthread_mutex_lock(&mx);
	self_of[i] = pthread_self();
	(void)pthread_mutex_unlock(&mx);
	post(&ready[i], 1);
	await(&release, 1);	/* stay alive while the IDs are compared */
	return NULL;
}

int main(void)
{
	static int idx[2] = { 0, 1 };
	pthread_t t[2], me, s0, s1;
	int r, i, ok0, ok1, diff, not_me;

	pc_begin("XSH.pthread_create.002");
	me = pthread_self();
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_create(&t[i], NULL, start, &idx[i]);
		if (r != 0)
			pc_unresolved("pthread_create failed: %s", pc_errname(r));
	}
	await(&ready[0], 1);
	await(&ready[1], 1);
	(void)pthread_mutex_lock(&mx);
	s0 = self_of[0];
	s1 = self_of[1];
	(void)pthread_mutex_unlock(&mx);
	ok0 = pthread_equal(t[0], s0) != 0;
	ok1 = pthread_equal(t[1], s1) != 0;
	diff = pthread_equal(t[0], t[1]) == 0;
	not_me = pthread_equal(t[0], me) == 0 && pthread_equal(t[1], me) == 0;
	pc_check(ok0 && ok1, "the stored ID does not equal the new thread's pthread_self() (first %d, second %d)", ok0, ok1);
	pc_check(diff, "two live threads were given equal IDs");
	pc_check(not_me, "a new thread's ID equals its creator's");
	post(&release, 1);
	for (i = 0; i < 2; i++)
		(void)pthread_join(t[i], NULL);
	pc_pass();
}
