/*
 * A thread owns a pthread_mutex_t after its lock calls succeed and after
 * a condition wait returns, and not after unlock or while it is blocked in
 * a condition wait.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.006
 *
 * Ownership is observed through pthread_mutex_trylock() made by another
 * thread: EBUSY while the mutex is owned, 0 (and then unlocked at once)
 * when it is not.
 */
#include "pcth.h"

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

static void *probe(void *a)
{
	int r = pthread_mutex_trylock(a);

	if (r == 0)
		(void)pthread_mutex_unlock(a);
	return (void *)(long)r;
}

/* What another thread's pthread_mutex_trylock() returns for m. */
static int other(void)
{
	pthread_t t = start(probe, &m);
	void *st;

	NEED(join_within(t, 10000, &st), "the probe thread did not end");
	return (int)(long)st;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void *waiter(void *a)
{
	struct timespec ts = ts_after(CLOCK_REALTIME, 2000);
	int r;

	(void)a;
	(void)pthread_mutex_lock(&m);
	flag_set(2, 1);
	r = pthread_cond_timedwait(&cv, &m, &ts);	/* nobody signals: times out */
	flag_set(5, r);
	flag_set(3, 1);
	while (!flag_get(4))
		sleep_ms(1);
	(void)pthread_mutex_unlock(&m);
	return NULL;
}

int main(void)
{
	struct timespec ts;
	pthread_t w;
	int r;

	pc_begin("XSH.2.9-threads.006");
	guard(120);
	pc_test();

	r = pthread_mutex_lock(&m);
	NEED(r == 0, "pthread_mutex_lock failed");
	r = other();
	pc_check(r == EBUSY, "after pthread_mutex_lock() another thread's trylock returned %s, expected EBUSY", rn(r));
	NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");
	r = other();
	pc_check(r == 0, "after pthread_mutex_unlock() another thread's trylock returned %s, expected 0", rn(r));

	r = pthread_mutex_trylock(&m);
	NEED(r == 0, "pthread_mutex_trylock failed");
	r = other();
	pc_check(r == EBUSY, "after pthread_mutex_trylock() another thread's trylock returned %s, expected EBUSY", rn(r));
	NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");

	ts = ts_after(CLOCK_REALTIME, 10000);
	r = pthread_mutex_timedlock(&m, &ts);
	NEED(r == 0, "pthread_mutex_timedlock of a free mutex failed: %s", rn(r));
	r = other();
	pc_check(r == EBUSY, "after pthread_mutex_timedlock() another thread's trylock returned %s, expected EBUSY", rn(r));
	NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");

	ts = ts_after(CLOCK_MONOTONIC, 10000);
	r = pthread_mutex_clocklock(&m, CLOCK_MONOTONIC, &ts);
	NEED(r == 0, "pthread_mutex_clocklock of a free mutex failed: %s", rn(r));
	r = other();
	pc_check(r == EBUSY, "after pthread_mutex_clocklock() another thread's trylock returned %s, expected EBUSY", rn(r));
	NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");
	r = other();
	pc_check(r == 0, "after the last unlock another thread's trylock returned %s, expected 0", rn(r));

	/* The waiter holds m until it blocks in the wait; main gets m only then. */
	w = start(waiter, NULL);
	NEED(wait_flag(2, 1, 10000), "the waiter did not start");
	r = pthread_mutex_lock(&m);
	NEED(r == 0, "pthread_mutex_lock failed");
	pc_check(flag_get(3) == 0, "the mutex was taken from a thread that had not blocked in the wait, or the wait ended early");
	NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");
	NEED(wait_flag(3, 1, 10000), "the timed wait did not return");
	r = flag_get(5);
	pc_check(r == 0 || r == ETIMEDOUT, "pthread_cond_timedwait() returned %s", rn(r));
	r = other();
	pc_check(r == EBUSY, "after pthread_cond_timedwait() returned another thread's trylock returned %s, expected EBUSY (the waiter owns the mutex)", rn(r));
	flag_set(4, 1);
	NEED(pthread_join(w, NULL) == 0, "pthread_join failed");
	r = other();
	pc_check(r == 0, "after the waiter unlocked another thread's trylock returned %s, expected 0", rn(r));
	pc_pass();
}
