/*
 * A locked mutex or write-locked read-write lock has one owner at a time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.005
 *
 * Four threads each take the lock 20000 times and, inside it, count the
 * threads inside (never above 1) and add one to a shared counter in two
 * steps (read, then write), so that a second owner would lose updates.
 * Done with a pthread_mutex_t, with an mtx_t and with a write-locked
 * pthread_rwlock_t; for the read-write lock two more threads take read
 * locks 20000 times and see no writer inside.
 */
#include "pcth.h"

#include <threads.h>

#define NT 4
#define LOOPS 20000

static pthread_mutex_t pm = PTHREAD_MUTEX_INITIALIZER;
static mtx_t cm;
static pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
static volatile int inside;
static volatile long counter;
static volatile int bad;
static volatile int readers_saw_writer;

static void bump(void)
{
	long c;

	if (inside++ != 0)
		bad = 1;
	c = counter;
	if (inside != 1)
		bad = 1;
	counter = c + 1;
	inside--;
}

static void *use_pm(void *a)
{
	int i;

	(void)a;
	for (i = 0; i < LOOPS; i++) {
		(void)pthread_mutex_lock(&pm);
		bump();
		(void)pthread_mutex_unlock(&pm);
	}
	return NULL;
}

static void *use_mtx(void *a)
{
	int i;

	(void)a;
	for (i = 0; i < LOOPS; i++) {
		(void)mtx_lock(&cm);
		bump();
		(void)mtx_unlock(&cm);
	}
	return NULL;
}

static void *use_rw(void *a)
{
	int i;

	(void)a;
	for (i = 0; i < LOOPS; i++) {
		(void)pthread_rwlock_wrlock(&rw);
		bump();
		(void)pthread_rwlock_unlock(&rw);
	}
	return NULL;
}

static void *read_rw(void *a)
{
	int i;

	(void)a;
	for (i = 0; i < LOOPS; i++) {
		(void)pthread_rwlock_rdlock(&rw);
		if (inside != 0)
			readers_saw_writer = 1;
		(void)pthread_rwlock_unlock(&rw);
	}
	return NULL;
}

static void run(void *(*fn)(void *), int n, const char *what)
{
	pthread_t t[8];
	int i;

	counter = 0;
	inside = 0;
	bad = 0;
	for (i = 0; i < n; i++)
		t[i] = start(fn, NULL);
	for (i = 0; i < n; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	pc_check(bad == 0, "%s: two threads were inside the lock at once", what);
	pc_check(counter == (long)n * LOOPS, "%s: the counter is %ld, expected %ld (updates lost)", what, (long)counter, (long)n * LOOPS);
}

int main(void)
{
	pthread_t t[8];
	int i;

	pc_begin("XSH.2.9-threads.005");
	guard(120);
	NEED(mtx_init(&cm, mtx_plain) == thrd_success, "mtx_init failed");
	pc_test();
	run(use_pm, NT, "pthread_mutex_t");
	run(use_mtx, NT, "mtx_t");
	counter = 0;
	inside = 0;
	bad = 0;
	for (i = 0; i < NT; i++)
		t[i] = start(use_rw, NULL);
	t[NT] = start(read_rw, NULL);
	t[NT + 1] = start(read_rw, NULL);
	for (i = 0; i < NT + 2; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	pc_check(bad == 0, "pthread_rwlock_t: two writers were inside the lock at once");
	pc_check(counter == (long)NT * LOOPS, "pthread_rwlock_t: the counter is %ld, expected %ld", (long)counter, (long)NT * LOOPS);
	pc_check(readers_saw_writer == 0, "pthread_rwlock_t: a reader held the lock while a writer was inside");
	pc_pass();
}
