/*
 * feclearexcept(0) succeeds and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.004
 */
#include "fh.h"

int main(void)
{
	int e, r;

	pc_begin("XSH.feclearexcept.004");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	pc_test();
	fh_raise(e);
	r = feclearexcept(0);
	pc_check(r == 0, "feclearexcept(0) returned %d, expected 0", r);
	pc_check((fetestexcept(e) & e) != 0, "feclearexcept(0) cleared exception 0x%x", e);
	fh_clearall();
	pc_pass();
}
