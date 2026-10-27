/*
 * atomic_flag_clear_explicit() clears a set flag with each memory order
 * that is allowed for it (relaxed, release, seq_cst).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_clear.002
 */
#define PCAT_ID "XSH.atomic_flag_clear.002"
#include "pcat.h"
#ifndef PCAT_NONE

static const memory_order clear_orders[3] = {
	memory_order_relaxed, memory_order_release, memory_order_seq_cst
};

int main(void)
{
	int o, k;

	pc_begin(PCAT_ID);
	pc_test();
	for (o = 0; o < 3; o++) {
		memory_order ord = clear_orders[o];
		atomic_flag f = ATOMIC_FLAG_INIT;
		atomic_flag *volatile p = &f;

		pc_check(!atomic_flag_test_and_set(p), "order %s: a flag from ATOMIC_FLAG_INIT was not clear",
		    pcat_oname(ord));
		pc_check(atomic_flag_test_and_set(p), "order %s: the flag did not stay set", pcat_oname(ord));
		atomic_flag_clear_explicit(p, ord);
		pc_check(!atomic_flag_test_and_set(p), "order %s: the flag was not clear after the call",
		    pcat_oname(ord));
		for (k = 0; k < 3; k++) {
			atomic_flag_clear_explicit(p, ord);
			pc_check(!atomic_flag_test_and_set(p), "order %s: round %d: not clear", pcat_oname(ord), k);
		}
	}
	pc_pass();
}
#endif
