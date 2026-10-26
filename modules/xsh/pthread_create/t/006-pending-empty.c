/*
 * A signal pending for the creating thread is not pending for the new one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.006
 * MUTANT    pthread_create.pending_copied
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
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

static int seen = -1;

static void *start(void *arg)
{
	sigset_t p;
	int v = -2;

	(void)arg;
	if (sigpending(&p) == 0)
		v = sigismember(&p, SIGUSR1);
	post(&seen, v);
	return NULL;
}

int main(void)
{
	sigset_t blk, p;
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_create.006");
	sigemptyset(&blk);
	sigaddset(&blk, SIGUSR1);
	pc_check(pthread_sigmask(SIG_BLOCK, &blk, NULL) == 0, "pthread_sigmask failed");
	pc_check(pthread_kill(pthread_self(), SIGUSR1) == 0, "pthread_kill failed");
	pc_check(sigpending(&p) == 0 && sigismember(&p, SIGUSR1) == 1, "SIGUSR1 is not pending for this thread");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	if (peek(&seen) == -2)
		pc_unresolved("sigpending failed in the new thread");
	pc_check(peek(&seen) == 0, "SIGUSR1, pending only for the creating thread, is pending in the new thread");
	pc_pass();
}
