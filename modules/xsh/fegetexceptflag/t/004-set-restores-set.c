/*
 * fesetexceptflag() restores a flag that was set when stored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.004
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;
	int i, e;

	pc_begin("XSH.fegetexceptflag.004");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		fh_clearall();
		fh_raise(e);
		if (fegetexceptflag(&f, e) != 0)
			pc_unresolved("fegetexceptflag(, 0x%x) failed", e);
		fh_clearall();
		if (fesetexceptflag(&f, e) != 0)
			pc_fail("fesetexceptflag(, 0x%x) failed", e);
		pc_check((fetestexcept(e) & e) != 0, "exception 0x%x not set after fesetexceptflag() with a stored set flag", e);
	}
	fh_clearall();
	pc_pass();
}
