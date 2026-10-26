/*
 * pthread_key_delete() can be called from a destructor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_delete.004
 * MUTANT    pthread_key_delete.in_dtor_fails
 * MUTANT    pthread_key_delete.dtor_kept
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

static pthread_key_t ka, kb;
static int a_obj, b_obj, del_ret = -1, set_failed;
static char order[16];

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

static void da(void *v)
{
	(void)v;
	note('A');
	post(&del_ret, pthread_key_delete(kb));
}

static void db(void *v)
{
	(void)v;
	note('B');
}

static void *start(void *arg)
{
	(void)arg;
	if (pthread_setspecific(ka, &a_obj) != 0 || pthread_setspecific(kb, &b_obj) != 0)
		post(&set_failed, 1);
	return NULL;
}

int main(void)
{
	char got[16];
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_key_delete.004");
	r = pthread_key_create(&ka, da);
	if (r == 0)
		r = pthread_key_create(&kb, db);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	if (peek(&set_failed))
		pc_unresolved("pthread_setspecific failed in the thread");
	(void)pthread_mutex_lock(&mx);
	(void)snprintf(got, sizeof got, "%s", order);
	(void)pthread_mutex_unlock(&mx);
	if (strchr(got, 'A') == NULL)
		pc_unresolved("the destructor that deletes the key did not run (\"%s\")", got);
	pc_check(peek(&del_ret) == 0, "pthread_key_delete in a destructor returned %s, expected 0",
		 pc_errname(peek(&del_ret)));
	pc_check(strchr(strchr(got, 'A'), 'B') == NULL,
		 "the deleted key's destructor ran after its deletion (order \"%s\")", got);
	pc_pass();
}
