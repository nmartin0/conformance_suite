/*
 * A new thread starts with its creator's signal mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.005
 * MUTANT    pthread_create.mask_cleared
 * MUTANT    pthread_create.mask_full
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

static const int sigs[4] = { SIGUSR1, SIGALRM, SIGUSR2, SIGTERM };
static int seen[4] = { -1, -1, -1, -1 };

static void *start(void *arg)
{
	sigset_t cur;
	int i;

	(void)arg;
	if (pthread_sigmask(SIG_BLOCK, NULL, &cur) != 0)
		return NULL;
	(void)pthread_mutex_lock(&mx);
	for (i = 0; i < 4; i++)
		seen[i] = sigismember(&cur, sigs[i]);
	(void)pthread_mutex_unlock(&mx);
	return NULL;
}

int main(void)
{
	sigset_t blk, unblk;
	pthread_t t;
	int r, i, s[4];

	pc_begin("XSH.pthread_create.005");
	sigemptyset(&blk);
	sigaddset(&blk, SIGUSR1);
	sigaddset(&blk, SIGALRM);
	sigemptyset(&unblk);
	sigaddset(&unblk, SIGUSR2);
	sigaddset(&unblk, SIGTERM);
	pc_check(pthread_sigmask(SIG_BLOCK, &blk, NULL) == 0, "pthread_sigmask(SIG_BLOCK) failed");
	pc_check(pthread_sigmask(SIG_UNBLOCK, &unblk, NULL) == 0, "pthread_sigmask(SIG_UNBLOCK) failed");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	(void)pthread_mutex_lock(&mx);
	for (i = 0; i < 4; i++)
		s[i] = seen[i];
	(void)pthread_mutex_unlock(&mx);
	if (s[0] == -1)
		pc_unresolved("the new thread could not read its mask");
	pc_check(s[0] == 1 && s[1] == 1, "the new thread does not have SIGUSR1 and SIGALRM blocked (%d, %d)", s[0], s[1]);
	pc_check(s[2] == 0 && s[3] == 0, "the new thread has SIGUSR2 or SIGTERM blocked (%d, %d), unlike its creator", s[2], s[3]);
	pc_pass();
}
