/*
 * A thread detaches itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_detach.003
 * MUTANT    pthread_detach.self_edeadlk
 * MUTANT    pthread_detach.self_ends
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

static int result = -1, release, ended;	/* ended: 1 finished, 2 ended otherwise */

static void on_end(void *arg)
{
	(void)arg;
	post(&ended, 2);
}

static void *start(void *arg)
{
	int r;

	(void)arg;
	pthread_cleanup_push(on_end, NULL);
	r = pthread_detach(pthread_self());
	post(&result, r);
	await(&release, 1);
	pthread_cleanup_pop(0);
	post(&ended, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, e;

	pc_begin("XSH.pthread_detach.003");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_mutex_lock(&mx);
	while (result == -1 && ended == 0)
		(void)pthread_cond_wait(&cv, &mx);
	r = result;
	(void)pthread_mutex_unlock(&mx);
	pc_check(r == 0, "pthread_detach(pthread_self()) returned %s, expected 0", r == -1 ? "nothing" : pc_errname(r));
	post(&release, 1);
	(void)pthread_mutex_lock(&mx);
	while (ended == 0)
		(void)pthread_cond_wait(&cv, &mx);
	e = ended;
	(void)pthread_mutex_unlock(&mx);
	pc_check(e == 1, "the thread did not go on running after detaching itself");
	pc_pass();
}
