/*
 * fetestexcept() returns the OR of all queried exceptions that are raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.005
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fetestexcept.005");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	fh_raise(fh_e2);
	pc_test();
	r = fetestexcept(fh_e1 | fh_e2);
	pc_check(r == (fh_e1 | fh_e2), "fetestexcept(0x%x|0x%x) returned 0x%x with both raised", fh_e1, fh_e2, r);
	fh_clearall();
	pc_pass();
}
