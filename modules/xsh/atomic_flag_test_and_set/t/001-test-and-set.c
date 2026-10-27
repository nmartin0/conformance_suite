/*
 * A flag made with ATOMIC_FLAG_INIT is clear: atomic_flag_test_and_set()
 * returns false for it, sets it, and returns true on every later call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_test_and_set.001
 */
#define PCAT_ID "XSH.atomic_flag_test_and_set.001"
#include "pcat.h"
#ifndef PCAT_NONE

static atomic_flag sflag = ATOMIC_FLAG_INIT;

struct holder { int pad; atomic_flag f; int tail; };

int main(void)
{
	atomic_flag aflag = ATOMIC_FLAG_INIT;
	atomic_flag arr[3] = { ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT };
	struct holder h = { 1, ATOMIC_FLAG_INIT, 2 };
	atomic_flag *volatile ps = &sflag;
	atomic_flag *volatile pa = &aflag;
	atomic_flag *volatile pe = arr;
	atomic_flag *volatile ph = &h.f;
	int k;

	pc_begin(PCAT_ID);
	pc_test();
	pc_check(!atomic_flag_test_and_set(pa), "an automatic flag from ATOMIC_FLAG_INIT was not clear");
	pc_check(atomic_flag_test_and_set(pa), "the flag was not set by the first call");
	pc_check(atomic_flag_test_and_set(pa), "the flag did not stay set (third call)");
	pc_check(atomic_flag_test_and_set(pa), "the flag did not stay set (fourth call)");
	pc_check(!atomic_flag_test_and_set(ps), "a static flag from ATOMIC_FLAG_INIT was not clear");
	pc_check(atomic_flag_test_and_set(ps), "the static flag was not set by the first call");
	pc_check(!atomic_flag_test_and_set(ph), "a flag member from ATOMIC_FLAG_INIT was not clear");
	pc_check(atomic_flag_test_and_set(ph), "the flag member was not set by the first call");
	pc_check(h.pad == 1 && h.tail == 2, "the neighbours of a flag member changed");
	for (k = 0; k < 3; k++)
		pc_check(!atomic_flag_test_and_set(&pe[k]), "array flag %d from ATOMIC_FLAG_INIT was not clear", k);
	for (k = 0; k < 3; k++)
		pc_check(atomic_flag_test_and_set(&pe[k]), "array flag %d was not set by its first call", k);
	pc_pass();
}
#endif
