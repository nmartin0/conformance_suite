/*
 * pthread_detach() of a live thread does not terminate it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_detach.001
 * MUTANT    pthread_detach.cancels
 * MUTANT    pthread_detach.einval_live
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

static void unlock_mx(void *arg)
{
	(void)arg;
	(void)pthread_mutex_unlock(&mx);
}

/* As await(), but a cancellation in pthread_cond_wait() releases the lock. */
static void await_c(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	pthread_cleanup_push(unlock_mx, NULL);
	while (*p != v)
		(void)pthread_cond_wait(&cv, &mx);
	pthread_cleanup_pop(1);
}

static int started, release, ended;	/* ended: 1 finished, 2 canceled */

static void on_cancel(void *arg)
{
	(void)arg;
	post(&ended, 2);
}

static void *start(void *arg)
{
	(void)arg;
	pthread_cleanup_push(on_cancel, NULL);
	post(&started, 1);
	await_c(&release, 1);	/* pthread_cond_wait(): a cancellation point */
	pthread_cleanup_pop(0);
	post(&ended, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, e;

	pc_begin("XSH.pthread_detach.001");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&started, 1);
	r = pthread_detach(t);
	pc_check(r == 0, "pthread_detach of a live joinable thread returned %s, expected 0", pc_errname(r));
	sleep_ms(100);
	post(&release, 1);
	(void)pthread_mutex_lock(&mx);
	while (ended == 0)
		(void)pthread_cond_wait(&cv, &mx);
	e = ended;
	(void)pthread_mutex_unlock(&mx);
	pc_check(e == 1, "the detached thread was canceled instead of running on");
	pc_pass();
}
