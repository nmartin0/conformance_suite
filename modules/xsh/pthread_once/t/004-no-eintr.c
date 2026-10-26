/*
 * A caught signal does not make pthread_once() return EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_once.004
 * MUTANT    pthread_once.eintr
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
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

static pthread_once_t once = PTHREAD_ONCE_INIT;
static volatile sig_atomic_t caught;
static int entered, done, calling, ret = -1, saw_done = -1;

static void on_usr1(int sig)
{
	(void)sig;
	caught = 1;
}

static void slow_init(void)
{
	post(&entered, 1);
	sleep_ms(400);
	post(&done, 1);
}

static void never(void)
{
}

static void *runner(void *arg)
{
	(void)arg;
	(void)pthread_once(&once, slow_init);
	return NULL;
}

static void *waiter(void *arg)
{
	sigset_t s;
	int r;

	(void)arg;
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	(void)pthread_sigmask(SIG_UNBLOCK, &s, NULL);
	await(&entered, 1);
	post(&calling, 1);
	r = pthread_once(&once, never);
	(void)pthread_mutex_lock(&mx);
	ret = r;
	saw_done = done;
	(void)pthread_mutex_unlock(&mx);
	return NULL;
}

int main(void)
{
	struct sigaction sa;
	sigset_t s;
	pthread_t a, w;
	int r;

	pc_begin("XSH.pthread_once.004");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_usr1;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	pc_check(pthread_sigmask(SIG_BLOCK, &s, NULL) == 0, "pthread_sigmask failed");	/* only the waiter takes it */
	pc_test();
	r = pthread_create(&w, NULL, waiter, NULL);
	if (r == 0)
		r = pthread_create(&a, NULL, runner, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&calling, 1);
	sleep_ms(150);
	if (pthread_kill(w, SIGUSR1) != 0)
		pc_unresolved("pthread_kill failed");
	(void)pthread_join(w, NULL);
	(void)pthread_join(a, NULL);
	pc_check(peek(&ret) != EINTR, "pthread_once returned EINTR after a caught signal");
	pc_check(peek(&ret) == 0, "pthread_once returned %s, expected 0", pc_errname(peek(&ret)));
	pc_check(peek(&saw_done) == 1, "pthread_once returned before init_routine had completed");
	if (!caught)
		pc_unresolved("the waiting thread did not catch SIGUSR1");
	pc_pass();
}
