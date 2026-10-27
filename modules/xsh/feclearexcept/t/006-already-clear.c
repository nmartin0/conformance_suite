/*
 * feclearexcept() on flags that are already clear returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.006
 */
#include "fh.h"

int main(void)
{
	int i, e, r;

	pc_begin("XSH.feclearexcept.006");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	fh_clearall();
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		r = feclearexcept(e);
		pc_check(r == 0, "feclearexcept(0x%x) on a clear flag returned %d, expected 0", e, r);
	}
	pc_pass();
}
