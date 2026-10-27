/*
 * feclearexcept() returns zero when it clears a raised exception.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.001
 */
#include "fh.h"

int main(void)
{
	int i, e, r;

	pc_begin("XSH.feclearexcept.001");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		fh_raise(e);
		r = feclearexcept(e);
		pc_check(r == 0, "feclearexcept(0x%x) returned %d, expected 0", e, r);
		fh_clearall();
	}
	pc_pass();
}
