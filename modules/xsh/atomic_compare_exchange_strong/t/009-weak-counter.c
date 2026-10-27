/*
 * A counter advanced only by weak compare-and-exchange loops in several
 * threads reaches exactly the number of increments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.009
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.009"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define M 10000

static atomic_int counter;
static atomic_int gaveup;

static void *thread_main(void *arg)
{
	int j, exp = 0;
	unsigned n = 0;
	long long t0;

	(void)arg;
	pcat_wait_go();
	t0 = pcat_ms();
	for (j = 0; j < M; j++) {
		/* exp is this thread's belief of the current value; failures refresh it */
		while (!atomic_compare_exchange_weak(&counter, &exp, exp + 1)) {
			if ((++n & 0xffffu) == 0 && pcat_expired(t0, 30000)) {
				atomic_store(&gaveup, 1);
				return NULL;
			}
		}
		exp++;
		t0 = pcat_ms();		/* the time limit is for a stretch without progress */
	}
	return NULL;
}

int main(void)
{
	int fin;

	pc_begin(PCAT_ID);
	atomic_store(&counter, 0);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	fin = atomic_load(&counter);
	if (atomic_load(&gaveup))
		pc_unresolved("a weak exchange loop made no progress in 30 s (counter %d)", fin);
	pc_check(fin == T * M, "counter is %d after %d increments", fin, T * M);
	pc_pass();
}
#endif
