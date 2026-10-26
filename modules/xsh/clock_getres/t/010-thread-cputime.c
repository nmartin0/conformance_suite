/*
 * CLOCK_THREAD_CPUTIME_ID measures the calling thread's execution time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.010
 * MUTANT    clock_getres.thread_cputime_einval
 * MUTANT    clock_getres.thread_is_process
 * MUTANT    clock_getres.cputime_frozen
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

static long long ns_of(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0 || t.tv_nsec < 0 || t.tv_nsec >= 1000000000L)
		return -1;
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long wall(void)
{
	return ns_of(CLOCK_MONOTONIC);
}

/* Computes until the thread's own clock moved by `amount' ns (or 20 s pass): 1 if it did. */
static int compute(long long amount)
{
	volatile unsigned long spin = 0;
	long long c0 = ns_of(CLOCK_THREAD_CPUTIME_ID), c, w0 = wall();

	if (c0 < 0)
		return 0;
	do {
		int k;
		for (k = 0; k < 100000; k++)
			spin++;
		c = ns_of(CLOCK_THREAD_CPUTIME_ID);
	} while (c >= 0 && c - c0 < amount && wall() - w0 < 20000000000LL);
	return c >= 0 && c - c0 >= amount;
}

static void *worker(void *arg)
{
	*(int *)arg = compute(500000000LL);
	return NULL;
}

int main(void)
{
	struct timespec res;
	pthread_t t;
	long long m0, m1;
	int ok = -1, r;

	pc_begin("XSH.clock_getres.010");
	pc_test();
	pc_check(clock_getres(CLOCK_THREAD_CPUTIME_ID, &res) == 0,
	    "clock_getres(CLOCK_THREAD_CPUTIME_ID) failed: %s", pc_errname(errno));
	pc_check(ns_of(CLOCK_THREAD_CPUTIME_ID) >= 0, "clock_gettime(CLOCK_THREAD_CPUTIME_ID) failed: %s", pc_errname(errno));
	pc_check(compute(1), "CLOCK_THREAD_CPUTIME_ID did not advance while the thread computed for 20 s");
	m0 = ns_of(CLOCK_THREAD_CPUTIME_ID);
	r = pthread_create(&t, NULL, worker, &ok);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	(void)pthread_join(t, NULL);
	m1 = ns_of(CLOCK_THREAD_CPUTIME_ID);
	pc_check(ok == 1, "the second thread's CLOCK_THREAD_CPUTIME_ID did not reach 500 ms in 20 s");
	pc_check(m0 >= 0 && m1 >= 0, "clock_gettime(CLOCK_THREAD_CPUTIME_ID) failed");
	pc_check(m1 - m0 < 250000000LL,
	    "the main thread's CPU-time clock advanced %lld ms while it waited for a thread that computed 500 ms",
	    (m1 - m0) / 1000000LL);
	pc_pass();
}
