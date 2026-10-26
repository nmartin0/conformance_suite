/*
 * pthread_join() does not return EINTR when a signal is caught.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_join.005
 * MUTANT    pthread_join.eintr
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

static volatile sig_atomic_t caught;
static pthread_t joiner;
static int ret_obj, kill_failed, joining;

static void on_usr1(int sig)
{
	(void)sig;
	caught = 1;
}

/* SIGUSR1 is blocked here (inherited); it goes to the joining thread. */
static void *start(void *arg)
{
	(void)arg;
	await(&joining, 1);
	sleep_ms(200);	/* the initial thread is in pthread_join() by now */
	if (pthread_kill(joiner, SIGUSR1) != 0)
		post(&kill_failed, 1);
	sleep_ms(200);
	return &ret_obj;
}

int main(void)
{
	struct sigaction sa;
	sigset_t s;
	pthread_t t;
	void *v = NULL;
	int r;

	pc_begin("XSH.pthread_join.005");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_usr1;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;	/* no SA_RESTART */
	pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	pc_check(pthread_sigmask(SIG_BLOCK, &s, NULL) == 0, "pthread_sigmask failed");
	joiner = pthread_self();
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_sigmask(SIG_UNBLOCK, &s, NULL) != 0)
		pc_unresolved("pthread_sigmask failed");
	post(&joining, 1);
	r = pthread_join(t, &v);
	pc_check(r != EINTR, "pthread_join returned EINTR after a caught signal");
	pc_check(r == 0, "pthread_join returned %s, expected 0", pc_errname(r));
	if (peek(&kill_failed) || !caught)
		pc_unresolved("SIGUSR1 was not caught by the joining thread");
	pc_check(v == &ret_obj, "pthread_join gave %p, expected %p", v, (void *)&ret_obj);
	pc_pass();
}
