/*
 * A new thread does not inherit its creator's alternate signal stack.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.008
 * MUTANT    pthread_create.altstack_inherited
 * VARIANT   pthread_create.altstack_fresh
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
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

static char *base;
static size_t size;
static int verdict = -1;	/* 0 none or another stack, 1 the creator's, 2 query failed */

static void *start(void *arg)
{
	stack_t o;
	int v;

	(void)arg;
	if (sigaltstack(NULL, &o) != 0)
		v = 2;
	else if (o.ss_flags & SS_DISABLE)
		v = 0;
	else
		v = (char *)o.ss_sp >= base && (char *)o.ss_sp < base + size;
	post(&verdict, v);
	return NULL;
}

int main(void)
{
	stack_t ss;
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_create.008");
	size = (size_t)SIGSTKSZ * 2;
	base = malloc(size);
	pc_check(base != NULL, "malloc failed");
	ss.ss_sp = base;
	ss.ss_size = size;
	ss.ss_flags = 0;
	pc_check(sigaltstack(&ss, NULL) == 0, "sigaltstack failed: %s", pc_errname(errno));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	if (peek(&verdict) == 2)
		pc_unresolved("sigaltstack query failed in the new thread");
	pc_check(peek(&verdict) == 0, "the new thread has its creator's alternate signal stack");
	pc_pass();
}
