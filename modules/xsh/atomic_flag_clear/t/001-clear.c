/*
 * atomic_flag_clear() puts a set flag into the clear state: the next
 * atomic_flag_test_and_set() returns false, and sets it again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_clear.001
 */
#define PCAT_ID "XSH.atomic_flag_clear.001"
#include "pcat.h"
#ifndef PCAT_NONE

int main(void)
{
	atomic_flag f = ATOMIC_FLAG_INIT;
	atomic_flag *volatile p = &f;
	int k;

	pc_begin(PCAT_ID);
	/* observing the state needs test_and_set; a flag that does not set is no use here */
	pc_check(!atomic_flag_test_and_set(p), "a flag from ATOMIC_FLAG_INIT was not clear");
	pc_check(atomic_flag_test_and_set(p), "the flag did not stay set");
	pc_test();
	atomic_flag_clear(p);
	pc_check(!atomic_flag_test_and_set(p), "the flag was not clear after atomic_flag_clear");
	pc_check(atomic_flag_test_and_set(p), "the flag was not set again by test_and_set");
	for (k = 0; k < 5; k++) {
		atomic_flag_clear(p);
		pc_check(!atomic_flag_test_and_set(p), "round %d: not clear after atomic_flag_clear", k);
		pc_check(atomic_flag_test_and_set(p), "round %d: not set after test_and_set", k);
	}
	pc_pass();
}
#endif
