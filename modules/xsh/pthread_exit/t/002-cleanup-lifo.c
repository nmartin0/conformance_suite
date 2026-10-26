/*
 * pthread_exit() runs the cleanup handlers still pushed, last first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_exit.002
 * MUTANT    pthread_exit.skip_cleanup
 * MUTANT    pthread_exit.innermost_only
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
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

static char order[16];

static void handler(void *arg)
{
	size_t n;

	(void)pthread_mutex_lock(&mx);
	n = strlen(order);
	if (n < sizeof order - 1) {
		order[n] = *(const char *)arg;
		order[n + 1] = '\0';
	}
	(void)pthread_mutex_unlock(&mx);
}

static void *start(void *arg)
{
	(void)arg;
	pthread_cleanup_push(handler, "1");
	pthread_cleanup_push(handler, "2");
	pthread_cleanup_push(handler, "x");
	pthread_cleanup_pop(0);	/* popped, not executed: must not run */
	pthread_cleanup_push(handler, "3");
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	char got[16];
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_exit.002");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	(void)pthread_mutex_lock(&mx);
	(void)snprintf(got, sizeof got, "%s", order);
	(void)pthread_mutex_unlock(&mx);
	pc_check(strcmp(got, "321") == 0, "the cleanup handlers ran as \"%s\", expected \"321\"", got);
	pc_pass();
}
