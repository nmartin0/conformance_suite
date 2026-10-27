/*
 * fetestexcept() does not clear the flags it reads.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.007
 */
#include "fh.h"

int main(void)
{
	int e, r1, r2;

	pc_begin("XSH.fetestexcept.007");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	fh_clearall();
	fh_raise(e);
	pc_test();
	r1 = fetestexcept(e);
	r2 = fetestexcept(e);
	pc_check(r1 == e && r2 == e, "fetestexcept(0x%x) returned 0x%x then 0x%x on two calls", e, r1, r2);
	fh_clearall();
	pc_pass();
}
