/*
 * Concurrent atomic_fetch_or() and atomic_fetch_and() calls lose no
 * update: each of four threads sets and clears its own bit repeatedly and
 * always finds it as it left it, however the others interleave.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.010
 */
#define PCAT_ID "XSH.atomic_fetch_add.010"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define M 20000

static atomic_uint v;
static atomic_uint wrong;

static void *thread_main(void *arg)
{
	unsigned bit = 1u << (unsigned)(uintptr_t)arg;
	unsigned old;
	int j;

	pcat_wait_go();
	for (j = 0; j < M; j++) {
		old = atomic_fetch_or(&v, bit);
		if (old & bit)
			atomic_fetch_add(&wrong, 1);
		old = atomic_fetch_and_explicit(&v, ~bit, memory_order_acq_rel);
		if (!(old & bit))
			atomic_fetch_add(&wrong, 1);
	}
	return NULL;
}

int main(void)
{
	pc_begin(PCAT_ID);
	atomic_store(&v, 0);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	pc_check(atomic_load(&wrong) == 0, "a thread found its own bit changed by another thread (%u times)",
	    atomic_load(&wrong));
	pc_check(atomic_load(&v) == 0, "final value is %#x, expected 0", atomic_load(&v));
	pc_pass();
}
#endif
