/*
 * Concurrent atomic_fetch_add() calls lose no update and each returns a
 * distinct previous value; the same for atomic_fetch_sub().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.008
 */
#define PCAT_ID "XSH.atomic_fetch_add.008"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define M 10000
#define TOTAL (T * M)

static atomic_int counter;
static atomic_llong wide;
static atomic_int seen[TOTAL + 1];
static atomic_int outofrange;
static int phase;	/* 0: add, 1: sub; set before the threads start */

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int j, old;

	pcat_wait_go();
	for (j = 0; j < M; j++) {
		if (phase == 0) {
			old = (idx & 1) ? atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed)
			                : atomic_fetch_add(&counter, 1);
			atomic_fetch_add(&wide, 2);
			if (old < 0 || old >= TOTAL)
				atomic_fetch_add(&outofrange, 1);
			else
				atomic_fetch_add(&seen[old], 1);
		} else {
			old = (idx & 1) ? atomic_fetch_sub_explicit(&counter, 1, memory_order_acq_rel)
			                : atomic_fetch_sub(&counter, 1);
			atomic_fetch_sub(&wide, 2);
			if (old < 1 || old > TOTAL)
				atomic_fetch_add(&outofrange, 1);
			else
				atomic_fetch_add(&seen[old - 1], 1);
		}
	}
	return NULL;
}

static void check_round(const char *what, int want_counter, long long want_wide)
{
	int k;

	pc_check(atomic_load(&outofrange) == 0, "%s: a call returned an impossible previous value", what);
	pc_check(atomic_load(&counter) == want_counter, "%s: counter is %d, expected %d", what,
	    atomic_load(&counter), want_counter);
	pc_check(atomic_load(&wide) == want_wide, "%s: wide counter is %lld, expected %lld", what,
	    (long long)atomic_load(&wide), want_wide);
	for (k = 0; k < TOTAL; k++)
		pc_check(atomic_load(&seen[k]) == 1, "%s: previous value %d was returned %d times, not once", what,
		    k, atomic_load(&seen[k]));
}

int main(void)
{
	int k;

	pc_begin(PCAT_ID);
	atomic_store(&counter, 0);
	atomic_store(&wide, 0);
	pc_test();
	phase = 0;
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	check_round("add", TOTAL, 2LL * TOTAL);
	for (k = 0; k <= TOTAL; k++)
		atomic_store(&seen[k], 0);
	phase = 1;
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	check_round("sub", 0, 0);
	pc_pass();
}
#endif
