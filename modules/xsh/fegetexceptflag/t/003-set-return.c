/*
 * fesetexceptflag() returns zero for a value stored by fegetexceptflag().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.003
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;
	int i, e, r;

	pc_begin("XSH.fegetexceptflag.003");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		if (fegetexceptflag(&f, e) != 0)
			pc_unresolved("fegetexceptflag(, 0x%x) failed", e);
		r = fesetexceptflag(&f, e);
		pc_check(r == 0, "fesetexceptflag(, 0x%x) returned %d, expected 0", e, r);
	}
	pc_pass();
}
