/*
 * pthread_self() in the initial thread is stable and its own.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_self.002
 * MUTANT    pthread_self.constant
 * MUTANT    pthread_self.fresh_each_call
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

static pthread_t seen;
static int ready, release;

static void *start(void *arg)
{
	pthread_t me = pthread_self();

	(void)arg;
	(void)pthread_mutex_lock(&mx);
	seen = me;
	(void)pthread_mutex_unlock(&mx);
	post(&ready, 1);
	await(&release, 1);
	return NULL;
}

int main(void)
{
	pthread_t a, b, c, s, t;
	int r, ab, ac, as;

	pc_begin("XSH.pthread_self.002");
	pc_test();
	a = pthread_self();
	b = pthread_self();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&ready, 1);
	c = pthread_self();
	(void)pthread_mutex_lock(&mx);
	s = seen;
	(void)pthread_mutex_unlock(&mx);
	ab = pthread_equal(a, b) != 0;
	ac = pthread_equal(a, c) != 0;
	as = pthread_equal(a, s) == 0;
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(ab, "two calls of pthread_self() in the initial thread gave different IDs");
	pc_check(ac, "pthread_self() in the initial thread changed after another thread was created");
	pc_check(as, "the initial thread's pthread_self() equals the other live thread's");
	pc_pass();
}
