/*
 * pthread_equal() and PTHREAD_NULL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_equal.003
 * MUTANT    pthread_equal.null_matches
 * MUTANT    pthread_equal.null_unequal
 * VARIANT   pthread_equal.null_zero
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

static pthread_t none = PTHREAD_NULL;	/* a valid initializer */
static int release;

static void *start(void *arg)
{
	(void)arg;
	await(&release, 1);
	return NULL;
}

int main(void)
{
	pthread_t me, t, n2 = PTHREAD_NULL;
	int r, e[5];

	pc_begin("XSH.pthread_equal.003");
	me = pthread_self();
	r = pthread_create(&t, NULL, start, NULL);
	pc_check(r == 0, "pthread_create failed: %s", pc_errname(r));
	pc_test();
	/* the function (not only a macro of the same name) is checked too */
	e[0] = pthread_equal(none, me) || (pthread_equal)(none, me);
	e[1] = pthread_equal(me, none) || (pthread_equal)(me, none);
	e[2] = pthread_equal(t, n2) || (pthread_equal)(t, n2);
	e[3] = pthread_equal(n2, t) || (pthread_equal)(n2, t);
	e[4] = pthread_equal(none, n2) && (pthread_equal)(none, n2);
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(e[0] == 0 && e[1] == 0, "PTHREAD_NULL compared equal to the calling thread's ID (%d, %d)", e[0], e[1]);
	pc_check(e[2] == 0 && e[3] == 0, "PTHREAD_NULL compared equal to another live thread's ID (%d, %d)", e[2], e[3]);
	pc_check(e[4] != 0, "pthread_equal(PTHREAD_NULL, PTHREAD_NULL) returned 0");
	pc_pass();
}
