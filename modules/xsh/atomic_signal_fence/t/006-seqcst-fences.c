/*
 * Two threads each store 1 to their own object, run a seq_cst fence, and
 * load the other's object (all relaxed): they never both load 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_signal_fence.006
 */
#define PCAT_ID "XSH.atomic_signal_fence.006"
#include "pcat.h"
#ifndef PCAT_NONE

#define R 20000

static atomic_int xs[R], ys[R];
static atomic_int r1[R], r2[R];
static atomic_int arrived[R];
static atomic_int gaveup;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int r;

	pcat_wait_go();
	for (r = 0; r < R; r++) {
		unsigned n = 0;
		long long t0 = pcat_ms();

		atomic_fetch_add(&arrived[r], 1);
		while (atomic_load(&arrived[r]) < 2) {
			pcat_relax(&n);
			if ((n & 0xfffu) == 0 && pcat_expired(t0, 20000)) {
				atomic_store(&gaveup, 1);
				return NULL;
			}
		}
		if (idx == 0) {
			atomic_store_explicit(&xs[r], 1, memory_order_relaxed);
			atomic_thread_fence(memory_order_seq_cst);
			atomic_store_explicit(&r1[r], atomic_load_explicit(&ys[r], memory_order_relaxed),
			    memory_order_relaxed);
		} else {
			atomic_store_explicit(&ys[r], 1, memory_order_relaxed);
			atomic_thread_fence(memory_order_seq_cst);
			atomic_store_explicit(&r2[r], atomic_load_explicit(&xs[r], memory_order_relaxed),
			    memory_order_relaxed);
		}
	}
	return NULL;
}

int main(void)
{
	int r;

	pc_begin(PCAT_ID);
	for (r = 0; r < R; r++) {
		atomic_store(&xs[r], 0);
		atomic_store(&ys[r], 0);
		atomic_store(&r1[r], 7);
		atomic_store(&r2[r], 7);
		atomic_store(&arrived[r], 0);
	}
	pc_test();
	if (pcat_par(2, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	if (atomic_load(&gaveup))
		pc_unresolved("the two threads did not meet within 20 s");
	for (r = 0; r < R; r++)
		pc_check(!(atomic_load(&r1[r]) == 0 && atomic_load(&r2[r]) == 0),
		    "round %d: both threads loaded 0 despite the seq_cst fences", r);
	pc_pass();
}
#endif
