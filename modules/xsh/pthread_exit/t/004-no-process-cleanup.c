/*
 * Thread termination releases no process resources and runs no atexit()
 * handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_exit.004
 * MUTANT    pthread_exit.unlocks_mutexes
 * MUTANT    pthread_exit.closes_fds
 * MUTANT    pthread_exit.runs_atexit
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
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

static pthread_mutex_t held = PTHREAD_MUTEX_INITIALIZER;
static int p[2] = { -1, -1 };
static int setup_failed;
static volatile int at_exit_ran;

static void at_exit_handler(void)
{
	at_exit_ran = 1;
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_mutex_lock(&held) != 0 || pipe(p) != 0 || write(p[1], "x", 1) != 1)
		post(&setup_failed, 1);
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t t;
	char c = 0;
	int r;

	pc_begin("XSH.pthread_exit.004");
	pc_check(atexit(at_exit_handler) == 0, "atexit failed");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	if (peek(&setup_failed))
		pc_unresolved("the thread could not lock the mutex or make the pipe");
	pc_check(at_exit_ran == 0, "an atexit() handler ran when the thread ended");
	r = pthread_mutex_trylock(&held);
	pc_check(r == EBUSY, "pthread_mutex_trylock of the mutex the ended thread held returned %s, "
		 "expected EBUSY: it was released", pc_errname(r));
	pc_check(write(p[1], "y", 1) == 1, "the pipe's write end, opened by the ended thread, is not open: %s",
		 pc_errname(errno));
	pc_check(read(p[0], &c, 1) == 1 && c == 'x', "the pipe's read end, opened by the ended thread, "
		 "no longer gives its data");
	pc_pass();
}
