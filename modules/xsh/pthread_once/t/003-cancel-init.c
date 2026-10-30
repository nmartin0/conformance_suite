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
#include <signal.h>
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


/* A stall must give a verdict, not a driver timeout: where it stalled says which. */
static volatile int stage;	/* 1 set-up, 2 cancel requested: the thread must end, 3 later steps */

static void on_stall(int sig)
{
	(void)sig;
	if (stage == 2)
		pc_fail("the thread blocked in %s was not canceled within 20 s of the request", "read() in an init_routine");
	pc_unresolved("the test stalled for 20 s at step %d (not the cancellation)", (int)stage);
}

static void arm(int st)
{
	struct sigaction sa;

	stage = st;
	sa.sa_handler = on_stall;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	(void)sigaction(SIGALRM, &sa, NULL);
	(void)alarm(20);
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
	arm(1);
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&entered, 1);
	sleep_ms(100);
	if (pthread_cancel(t) != 0)
		pc_unresolved("pthread_cancel failed");
	arm(2);
	if (pthread_join(t, &v) != 0 || v != PTHREAD_CANCELED)
		pc_unresolved("the thread in init_routine did not end canceled");
	arm(3);
	r = pthread_once(&once, second_init);
	pc_check(r == 0, "pthread_once after the canceled init_routine returned %s", pc_errname(r));
	(void)alarm(0);
	pc_check(peek(&second) == 1, "pthread_once after the canceled init_routine did not call its init_routine");
	pc_pass();
}
