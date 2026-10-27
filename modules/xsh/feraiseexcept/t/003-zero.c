/*
 * feraiseexcept(0) returns zero and raises nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feraiseexcept.003
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.feraiseexcept.003");
	fh_clearall();
	pc_test();
	r = feraiseexcept(0);
	pc_check(r == 0, "feraiseexcept(0) returned %d, expected 0", r);
	r = fetestexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "flags 0x%x set after feraiseexcept(0)", r);
	pc_pass();
}
