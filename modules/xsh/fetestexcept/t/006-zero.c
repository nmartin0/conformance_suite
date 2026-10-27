/*
 * fetestexcept(0) returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.006
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fetestexcept.006");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	fh_clearall();
	fh_raise(fh_exc[0]);
	pc_test();
	r = fetestexcept(0);
	pc_check(r == 0, "fetestexcept(0) returned 0x%x", r);
	fh_clearall();
	pc_pass();
}
