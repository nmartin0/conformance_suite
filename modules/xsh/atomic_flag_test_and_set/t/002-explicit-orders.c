/*
 * atomic_flag_test_and_set_explicit() returns the previous state and sets
 * the flag with every memory order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_test_and_set.002
 */
#define PCAT_ID "XSH.atomic_flag_test_and_set.002"
#include "pcat.h"
#ifndef PCAT_NONE

int main(void)
{
	int o;

	pc_begin(PCAT_ID);
	pc_test();
	for (o = 0; o < 6; o++) {
		memory_order ord = pcat_orders[o];
		atomic_flag f = ATOMIC_FLAG_INIT;
		atomic_flag *volatile p = &f;

		pc_check(!atomic_flag_test_and_set_explicit(p, ord), "order %s: a clear flag reported set",
		    pcat_oname(ord));
		pc_check(atomic_flag_test_and_set_explicit(p, ord), "order %s: the flag was not set by the first call",
		    pcat_oname(ord));
		pc_check(atomic_flag_test_and_set_explicit(p, ord), "order %s: the flag did not stay set",
		    pcat_oname(ord));
		/* the plain form sees the state the explicit form left */
		pc_check(atomic_flag_test_and_set(p), "order %s: the plain form saw the flag clear",
		    pcat_oname(ord));
	}
	pc_pass();
}
#endif
