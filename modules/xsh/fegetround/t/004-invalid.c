/*
 * fesetround() with a value that is no rounding direction macro returns nonzero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.004
 */
#include "fh.h"

int main(void)
{
	int r, m0;

	pc_begin("XSH.fegetround.004");
	m0 = fegetround();
	pc_test();
	/* -1 is negative; the macros are distinct non-negative values */
	r = fesetround(-1);
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_check(r != 0, "fesetround(-1) returned 0, expected a nonzero value");
	pc_pass();
}
