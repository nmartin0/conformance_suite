/*
 * fetestexcept() returns zero when no flag is set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.001
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fetestexcept.001");
	fh_clearall();
	pc_test();
	r = fetestexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "fetestexcept(FE_ALL_EXCEPT) returned 0x%x with all flags clear", r);
	pc_pass();
}
