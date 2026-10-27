/*
 * fetestexcept() leaves out raised exceptions that were not queried.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.004
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fetestexcept.004");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	fh_raise(fh_e2);
	pc_test();
	r = fetestexcept(fh_e1);
	pc_check(r == fh_e1, "fetestexcept(0x%x) returned 0x%x with both exceptions raised, expected 0x%x", fh_e1, r, fh_e1);
	fh_clearall();
	pc_pass();
}
