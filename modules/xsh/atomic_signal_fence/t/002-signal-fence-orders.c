/*
 * atomic_signal_fence() can be called with each of the six memory orders
 * and returns normally.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_signal_fence.002
 */
#define PCAT_ID "XSH.atomic_signal_fence.002"
#include "pcat.h"
#ifndef PCAT_NONE

int main(void)
{
	volatile int done = 0;
	int o;

	pc_begin(PCAT_ID);
	pc_test();
	for (o = 0; o < 6; o++) {
		atomic_signal_fence(pcat_orders[o]);
		done++;
	}
	atomic_signal_fence(memory_order_relaxed);
	atomic_signal_fence(memory_order_consume);
	atomic_signal_fence(memory_order_acquire);
	atomic_signal_fence(memory_order_release);
	atomic_signal_fence(memory_order_acq_rel);
	atomic_signal_fence(memory_order_seq_cst);
	pc_check(done == 6, "the calls did not all return");
	pc_pass();
}
#endif
