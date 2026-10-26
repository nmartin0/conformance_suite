/*
 * pthread_self() in a new thread returns that thread's ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_self.001
 * MUTANT    pthread_self.constant
 * MUTANT    pthread_self.zero_in_threads
 * VARIANT   pthread_create.late_store
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

static pthread_t seen;
static int ready, release;

static void *start(void *arg)
{
	pthread_t me = pthread_self();

	(void)arg;
	(void)pthread_mutex_lock(&mx);
	seen = me;
	(void)pthread_mutex_unlock(&mx);
	post(&ready, 1);
	await(&release, 1);	/* stays alive while the IDs are compared */
	return NULL;
}

int main(void)
{
	pthread_t t, creator, s;
	int r, same, other;

	pc_begin("XSH.pthread_self.001");
	pc_test();
	creator = pthread_self();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&ready, 1);
	(void)pthread_mutex_lock(&mx);
	s = seen;
	(void)pthread_mutex_unlock(&mx);
	same = pthread_equal(s, t) != 0;
	other = pthread_equal(s, creator) == 0;
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(same, "pthread_self() in the new thread differs from the ID pthread_create() stored");
	pc_check(other, "pthread_self() in the new thread equals its creator's");
	pc_pass();
}
