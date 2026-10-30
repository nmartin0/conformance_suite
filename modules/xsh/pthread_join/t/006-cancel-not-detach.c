/*
 * Canceling a thread blocked in pthread_join() leaves the target
 * joinable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_join.006
 * MUTANT    pthread_join.cancel_detaches
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
		pc_fail("the thread blocked in %s was not canceled within 20 s of the request", "pthread_join()");
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

static pthread_t target;
static int ret_obj, release, entered, canceled;

static void *target_fn(void *arg)
{
	(void)arg;
	await(&release, 1);
	return &ret_obj;
}

static void on_cancel(void *arg)
{
	(void)arg;
	post(&canceled, 1);
}

static void *joiner_fn(void *arg)
{
	void *v;

	(void)arg;
	pthread_cleanup_push(on_cancel, NULL);
	post(&entered, 1);
	(void)pthread_join(target, &v);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t j;
	void *v = NULL, *jv = NULL;
	int r;

	pc_begin("XSH.pthread_join.006");
	pc_test();
	arm(1);
	r = pthread_create(&target, NULL, target_fn, NULL);
	if (r == 0)
		r = pthread_create(&j, NULL, joiner_fn, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&entered, 1);
	sleep_ms(200);	/* let it block in pthread_join() */
	if (pthread_cancel(j) != 0)
		pc_unresolved("pthread_cancel failed");
	arm(2);
	await(&canceled, 1);
	if (pthread_join(j, &jv) != 0 || jv != PTHREAD_CANCELED)
		pc_unresolved("the joining thread did not end canceled");
	arm(3);
	post(&release, 1);
	r = pthread_join(target, &v);
	pc_check(r == 0, "after the canceled join, pthread_join of the target returned %s: it was detached",
		 pc_errname(r));
	(void)alarm(0);
	pc_check(v == &ret_obj, "pthread_join gave %p, expected %p", v, (void *)&ret_obj);
	pc_pass();
}
