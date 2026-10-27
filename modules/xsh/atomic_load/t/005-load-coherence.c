/*
 * Relaxed loads of an object whose value another thread only ever
 * increases never see the value go backwards (read-read coherence), and
 * the final value becomes visible.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_load.005
 */
#define PCAT_ID "XSH.atomic_load.005"
#include "pcat.h"
#ifndef PCAT_NONE

#define LAST 200000

static atomic_int counter;
static atomic_int regress;	/* loads that went backwards */
static atomic_int reached;	/* consumer saw LAST */

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int i;

	pcat_wait_go();
	if (idx == 0) {
		for (i = 1; i <= LAST; i++)
			atomic_store_explicit(&counter, i, memory_order_relaxed);
	} else {
		int prev = 0, a, b;
		long long t0 = pcat_ms();
		unsigned n = 0;

		while (prev < LAST && !pcat_expired(t0, 20000)) {
			a = atomic_load_explicit(&counter, memory_order_relaxed);
			b = atomic_load_explicit(&counter, memory_order_relaxed);
			if (a < prev || b < a)
				atomic_fetch_add(&regress, 1);
			prev = b > a ? b : a;
			pcat_relax(&n);
		}
		if (prev == LAST)
			atomic_store(&reached, 1);
	}
	return NULL;
}

int main(void)
{
	pc_begin(PCAT_ID);
	atomic_store(&counter, 0);
	pc_test();
	if (pcat_par(2, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	pc_check(atomic_load(&regress) == 0, "a later load saw an older value than an earlier one (%d times)",
	    atomic_load(&regress));
	if (!atomic_load(&reached))
		pc_unresolved("the final value was not seen within 20 s");
	pc_pass();
}
#endif
