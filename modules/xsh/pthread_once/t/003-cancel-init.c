/*
 * A canceled init_routine leaves once_control as if never used.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_once.003
 * MUTANT    pthread_once.cancel_done
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

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

static pthread_once_t once = PTHREAD_ONCE_INIT;
static int p[2], entered, second;

static void blocking_init(void)
{
	char c;

	post(&entered, 1);
	(void)read(p[0], &c, 1);	/* a cancellation point; no data ever comes */
}

static void second_init(void)
{
	(void)pthread_mutex_lock(&mx);
	second++;
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	(void)arg;
	(void)pthread_once(&once, blocking_init);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_once.003");
	pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&entered, 1);
	sleep_ms(100);
	if (pthread_cancel(t) != 0)
		pc_unresolved("pthread_cancel failed");
	if (pthread_join(t, &v) != 0 || v != PTHREAD_CANCELED)
		pc_unresolved("the thread in init_routine did not end canceled");
	r = pthread_once(&once, second_init);
	pc_check(r == 0, "pthread_once after the canceled init_routine returned %s", pc_errname(r));
	pc_check(peek(&second) == 1, "pthread_once after the canceled init_routine did not call its init_routine");
	pc_pass();
}
