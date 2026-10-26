/*
 * pthread_equal() of equal thread IDs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_equal.001
 * MUTANT    pthread_equal.always_zero
 * MUTANT    pthread_equal.inverted
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
	pthread_t me, copy, t, s;
	int r, e1, e2, e3;

	pc_begin("XSH.pthread_equal.001");
	me = pthread_self();
	copy = me;
	r = pthread_create(&t, NULL, start, NULL);
	pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	await(&ready, 1);
	(void)pthread_mutex_lock(&mx);
	s = seen;
	(void)pthread_mutex_unlock(&mx);
	pc_test();
	/* the function (not only a macro of the same name) is checked too */
	e1 = pthread_equal(me, me) && (pthread_equal)(me, me);
	e2 = pthread_equal(copy, me) && (pthread_equal)(copy, me);
	e3 = pthread_equal(t, s) && (pthread_equal)(t, s);
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(e1 != 0, "pthread_equal(self, self) returned 0");
	pc_check(e2 != 0, "pthread_equal of an ID and its copy returned 0");
	pc_check(e3 != 0, "pthread_equal of the stored ID and the thread's pthread_self() returned 0");
	pc_pass();
}
