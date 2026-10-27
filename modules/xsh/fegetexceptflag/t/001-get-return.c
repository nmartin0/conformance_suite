/*
 * fegetexceptflag() returns zero for each defined exception.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.001
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;
	int i, e, r;

	pc_begin("XSH.fegetexceptflag.001");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		r = fegetexceptflag(&f, e);
		pc_check(r == 0, "fegetexceptflag(, 0x%x) returned %d, expected 0", e, r);
	}
	pc_pass();
}
