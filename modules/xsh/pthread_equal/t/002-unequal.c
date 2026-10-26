/*
 * pthread_equal() of different thread IDs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_equal.002
 * MUTANT    pthread_equal.always_one
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

static int release;

static void *start(void *arg)
{
	(void)arg;
	await(&release, 1);	/* both threads stay alive while compared */
	return NULL;
}

int main(void)
{
	pthread_t me, t1, t2;
	int r, e[4];

	pc_begin("XSH.pthread_equal.002");
	me = pthread_self();
	r = pthread_create(&t1, NULL, start, NULL);
	if (r == 0)
		r = pthread_create(&t2, NULL, start, NULL);
	pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	pc_test();
	/* the function (not only a macro of the same name) is checked too */
	e[0] = pthread_equal(t1, t2) || (pthread_equal)(t1, t2);
	e[1] = pthread_equal(t2, t1) || (pthread_equal)(t2, t1);
	e[2] = pthread_equal(t1, me) || (pthread_equal)(t1, me);
	e[3] = pthread_equal(me, t2) || (pthread_equal)(me, t2);
	post(&release, 1);
	(void)pthread_join(t1, NULL);
	(void)pthread_join(t2, NULL);
	pc_check(e[0] == 0 && e[1] == 0, "pthread_equal of two live threads' IDs was non-zero (%d, %d)",
		 e[0], e[1]);
	pc_check(e[2] == 0 && e[3] == 0, "pthread_equal of a new thread's and the initial thread's IDs returned "
		 "%d and %d, expected 0", e[2], e[3]);
	pc_pass();
}
