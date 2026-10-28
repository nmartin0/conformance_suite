/*
 * A thread owns an mtx_t after mtx_lock(), mtx_trylock(), mtx_timedlock()
 * succeed and after cnd_wait() or cnd_timedwait() returns, and not after
 * mtx_unlock() or while blocked in a condition wait.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.007
 *
 * Ownership is observed through mtx_trylock() called by another thread:
 * thrd_busy while the mutex is owned, thrd_success (and then unlocked at
 * once) when it is not.
 */
#include "pcth.h"

#include <threads.h>

static mtx_t m;
static cnd_t cv;

static void *probe(void *a)
{
	int r = mtx_trylock(a);

	if (r == thrd_success)
		(void)mtx_unlock(a);
	return (void *)(long)r;
}

static int other(void)
{
	pthread_t t = start(probe, &m);
	void *st;

	NEED(join_within(t, 10000, &st), "the probe thread did not end");
	return (int)(long)st;
}

static const char *rn(int r)
{
	return r == thrd_success ? "thrd_success" : r == thrd_busy ? "thrd_busy" : r == thrd_timedout ? "thrd_timedout" : "thrd_error";
}

static void *waiter(void *a)
{
	int timed = *(int *)a, r;
	struct timespec ts;

	(void)mtx_lock(&m);
	flag_set(2, 1);
	if (timed) {
		(void)timespec_get(&ts, TIME_UTC);
		ts.tv_sec += 2;
		r = cnd_timedwait(&cv, &m, &ts);	/* nobody signals: times out */
	} else {
		r = cnd_wait(&cv, &m);
	}
	flag_set(5, r == thrd_success ? 1 : r == thrd_timedout ? 2 : 3);
	flag_set(3, 1);
	while (!flag_get(4))
		sleep_ms(1);
	(void)mtx_unlock(&m);
	return NULL;
}

int main(void)
{
	struct timespec ts;
	pthread_t w;
	int r, timed;

	pc_begin("XSH.2.9-threads.007");
	guard(120);
	NEED(mtx_init(&m, mtx_plain) == thrd_success, "mtx_init failed");
	NEED(cnd_init(&cv) == thrd_success, "cnd_init failed");
	pc_test();

	NEED(mtx_lock(&m) == thrd_success, "mtx_lock failed");
	r = other();
	pc_check(r == thrd_busy, "after mtx_lock() another thread's mtx_trylock() returned %s, expected thrd_busy", rn(r));
	NEED(mtx_unlock(&m) == thrd_success, "mtx_unlock failed");
	r = other();
	pc_check(r == thrd_success, "after mtx_unlock() another thread's mtx_trylock() returned %s, expected thrd_success", rn(r));

	NEED(mtx_trylock(&m) == thrd_success, "mtx_trylock of a free mutex failed");
	r = other();
	pc_check(r == thrd_busy, "after mtx_trylock() another thread's mtx_trylock() returned %s, expected thrd_busy", rn(r));
	NEED(mtx_unlock(&m) == thrd_success, "mtx_unlock failed");

	NEED(timespec_get(&ts, TIME_UTC) == TIME_UTC, "timespec_get failed");
	ts.tv_sec += 10;
	NEED(mtx_timedlock(&m, &ts) == thrd_success, "mtx_timedlock of a free mutex failed");
	r = other();
	pc_check(r == thrd_busy, "after mtx_timedlock() another thread's mtx_trylock() returned %s, expected thrd_busy", rn(r));
	NEED(mtx_unlock(&m) == thrd_success, "mtx_unlock failed");

	for (timed = 0; timed < 2; timed++) {
		flag_set(2, 0);
		flag_set(3, 0);
		flag_set(4, 0);
		flag_set(5, 0);
		w = start(waiter, &timed);
		NEED(wait_flag(2, 1, 10000), "the waiter did not start");
		/* The waiter holds m until it blocks in the wait; main gets m only then. */
		NEED(mtx_lock(&m) == thrd_success, "mtx_lock failed");
		pc_check(flag_get(3) == 0, "%s: the mutex was taken from a thread that had not blocked in the wait, or the wait ended early",
		    timed ? "cnd_timedwait" : "cnd_wait");
		if (!timed)
			NEED(cnd_signal(&cv) == thrd_success, "cnd_signal failed");
		NEED(mtx_unlock(&m) == thrd_success, "mtx_unlock failed");
		NEED(wait_flag(3, 1, 10000), "the wait did not return");
		r = flag_get(5);
		pc_check(r == 1 || (timed && r == 2), "%s returned an error", timed ? "cnd_timedwait()" : "cnd_wait()");
		r = other();
		pc_check(r == thrd_busy, "after %s returned another thread's mtx_trylock() returned %s, expected thrd_busy (the waiter owns the mutex)",
		    timed ? "cnd_timedwait()" : "cnd_wait()", rn(r));
		flag_set(4, 1);
		NEED(pthread_join(w, NULL) == 0, "pthread_join failed");
		r = other();
		pc_check(r == thrd_success, "after the waiter unlocked another thread's mtx_trylock() returned %s, expected thrd_success", rn(r));
	}
	pc_pass();
}
