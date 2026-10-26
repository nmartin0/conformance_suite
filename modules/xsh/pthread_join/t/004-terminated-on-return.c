/*
 * When pthread_join() returns, the target's cleanup handlers and
 * thread-specific data destructors have finished.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_join.004
 * MUTANT    pthread_join.early
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

static pthread_key_t key;
static int key_obj, ret_obj, cleanup_done, dtor_done, set_failed;

static void cleanup(void *arg)
{
	(void)arg;
	sleep_ms(200);
	post(&cleanup_done, 1);
}

static void dtor(void *v)
{
	(void)v;
	sleep_ms(100);
	post(&dtor_done, 1);
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(key, &key_obj) != 0)
		post(&set_failed, 1);
	pthread_cleanup_push(cleanup, NULL);
	pthread_exit(&ret_obj);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *v = NULL;
	int r, c, d;

	pc_begin("XSH.pthread_join.004");
	r = pthread_key_create(&key, dtor);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	r = pthread_join(t, &v);
	c = peek(&cleanup_done);
	d = peek(&dtor_done);
	pc_check(r == 0, "pthread_join returned %s, expected 0", pc_errname(r));
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	pc_check(c == 1, "pthread_join returned before the target's cleanup handler had finished");
	pc_check(d == 1, "pthread_join returned before the target's thread-specific data destructor had finished");
	pc_check(v == &ret_obj, "pthread_join gave %p, expected %p", v, (void *)&ret_obj);
	pc_pass();
}
