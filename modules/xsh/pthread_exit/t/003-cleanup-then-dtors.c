/*
 * Cleanup handlers run before thread-specific data destructors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_exit.003
 * MUTANT    pthread_exit.dtors_first
 * MUTANT    pthread_exit.skip_cleanup
 * MUTANT    pthread_exit.skip_dtors
 * VARIANT   pthread_create.runs_late
 * VARIANT   pthread_key_create.dtors_reversed
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

static pthread_key_t key;
static int key_obj, set_failed;
static char order[8];
static void *seen_in_handler, *dtor_arg;

static void note(char c)
{
	size_t n;

	(void)pthread_mutex_lock(&mx);
	n = strlen(order);
	if (n < sizeof order - 1) {
		order[n] = c;
		order[n + 1] = '\0';
	}
	(void)pthread_mutex_unlock(&mx);
}

static void handler(void *arg)
{
	void *v = pthread_getspecific(key);

	(void)arg;
	(void)pthread_mutex_lock(&mx);
	seen_in_handler = v;
	(void)pthread_mutex_unlock(&mx);
	note('H');
}

static void dtor(void *v)
{
	(void)pthread_mutex_lock(&mx);
	dtor_arg = v;
	(void)pthread_mutex_unlock(&mx);
	note('D');
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(key, &key_obj) != 0)
		post(&set_failed, 1);
	pthread_cleanup_push(handler, NULL);
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	char got[8];
	void *h, *d;
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_exit.003");
	r = pthread_key_create(&key, dtor);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	(void)pthread_mutex_lock(&mx);
	(void)snprintf(got, sizeof got, "%s", order);
	h = seen_in_handler;
	d = dtor_arg;
	(void)pthread_mutex_unlock(&mx);
	pc_check(strcmp(got, "HD") == 0, "handler (H) and destructor (D) ran as \"%s\", expected \"HD\"", got);
	pc_check(h == &key_obj, "the cleanup handler saw the key's value as %p, expected %p", h, (void *)&key_obj);
	pc_check(d == &key_obj, "the destructor got %p, expected %p", d, (void *)&key_obj);
	pc_pass();
}
