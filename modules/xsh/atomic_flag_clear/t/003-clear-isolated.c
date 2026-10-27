/*
 * Clearing a flag that is already clear leaves it clear, and clearing
 * one flag leaves other flags (here its neighbours in an array) set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_clear.003
 */
#define PCAT_ID "XSH.atomic_flag_clear.003"
#include "pcat.h"
#ifndef PCAT_NONE

int main(void)
{
	atomic_flag fl[5] = { ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT,
	    ATOMIC_FLAG_INIT };
	atomic_flag *volatile p = fl;
	atomic_flag lone = ATOMIC_FLAG_INIT;
	atomic_flag *volatile pl = &lone;
	int k, phase;

	pc_begin(PCAT_ID);
	for (k = 0; k < 5; k++)
		pc_check(!atomic_flag_test_and_set(&p[k]), "flag %d from ATOMIC_FLAG_INIT was not clear", k);
	pc_test();
	/* a clear flag stays clear */
	atomic_flag_clear(pl);
	atomic_flag_clear_explicit(pl, memory_order_release);
	pc_check(!atomic_flag_test_and_set(pl), "clearing a clear flag left it set");
	/* all of the array is set; clear one at a time, plain then explicit */
	for (phase = 0; phase < 2; phase++) {
		for (k = 0; k < 5; k++) {
			if (phase == 0)
				atomic_flag_clear(&p[2]);
			else
				atomic_flag_clear_explicit(&p[2], memory_order_seq_cst);
			pc_check(!atomic_flag_test_and_set(&p[2]), "phase %d: flag 2 was not cleared", phase);
			/* flag 2 is set again; the others were never touched, so all still set */
			for (int i = 0; i < 5; i++)
				pc_check(atomic_flag_test_and_set(&p[i]), "phase %d: flag %d was clear after clearing flag 2",
				    phase, i);
		}
	}
	pc_pass();
}
#endif
