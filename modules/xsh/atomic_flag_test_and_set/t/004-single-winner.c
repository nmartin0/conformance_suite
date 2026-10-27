/*
 * When several threads call atomic_flag_test_and_set() on the same clear
 * flag together, exactly one of them gets false (the previous state),
 * in every one of many rounds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_test_and_set.004
 */
#define PCAT_ID "XSH.atomic_flag_test_and_set.004"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define F4 ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT
#define F16 F4, F4, F4, F4
#define F64 F16, F16, F16, F16
#define F256 F64, F64, F64, F64
#define R 1024

static atomic_flag flags[R] = { F256, F256, F256, F256 };
static atomic_int arrived[R];
static atomic_int winners[R];
static atomic_int gaveup;

static void *thread_main(void *arg)
{
	int r;

	(void)arg;
	pcat_wait_go();
	for (r = 0; r < R; r++) {
		unsigned n = 0;
		long long t0 = pcat_ms();

		atomic_fetch_add(&arrived[r], 1);
		while (atomic_load(&arrived[r]) < T) {
			pcat_relax(&n);
			if ((n & 0xfffu) == 0 && pcat_expired(t0, 20000)) {
				atomic_store(&gaveup, 1);
				return NULL;
			}
		}
		if (!atomic_flag_test_and_set(&flags[r]))
			atomic_fetch_add(&winners[r], 1);
	}
	return NULL;
}

int main(void)
{
	int r;

	pc_begin(PCAT_ID);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	if (atomic_load(&gaveup))
		pc_unresolved("the threads did not meet within 20 s");
	for (r = 0; r < R; r++)
		pc_check(atomic_load(&winners[r]) == 1, "round %d: %d threads saw the flag clear, not exactly one", r,
		    atomic_load(&winners[r]));
	pc_pass();
}
#endif
