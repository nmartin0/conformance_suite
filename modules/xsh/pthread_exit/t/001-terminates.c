/*
 * pthread_exit() ends the thread and gives its value to pthread_join().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_exit.001
 * MUTANT    pthread_exit.value_null
 * MUTANT    pthread_exit.value_of_first
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

static int obj[2], after;

static void leave(void *v)
{
	pthread_exit(v);
}

static void *start(void *arg)
{
	leave(arg);
	post(&after, 1);	/* not reached */
	return NULL;
}

int main(void)
{
	pthread_t t[2];
	void *v[2] = { NULL, NULL };
	int r, i;

	pc_begin("XSH.pthread_exit.001");
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_create(&t[i], NULL, start, &obj[i]);
		if (r != 0)
			pc_unresolved("pthread_create failed: %s", pc_errname(r));
		r = pthread_join(t[i], &v[i]);
		if (r != 0)
			pc_unresolved("pthread_join failed: %s", pc_errname(r));
	}
	pc_check(peek(&after) == 0, "code after pthread_exit() ran");
	for (i = 0; i < 2; i++)
		pc_check(v[i] == &obj[i], "thread %d called pthread_exit(%p) but was joined as %p",
			 i, (void *)&obj[i], v[i]);
	pc_pass();
}
