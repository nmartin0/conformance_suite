/*
 * fesetexceptflag() with an empty set returns zero and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.007
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;
	int e, r;

	pc_begin("XSH.fegetexceptflag.007");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	fh_clearall();
	if (fegetexceptflag(&f, FE_ALL_EXCEPT) != 0)
		pc_unresolved("fegetexceptflag() failed");
	fh_raise(e);
	pc_test();
	r = fesetexceptflag(&f, 0);
	pc_check(r == 0, "fesetexceptflag(, 0) returned %d, expected 0", r);
	pc_check((fetestexcept(e) & e) != 0, "fesetexceptflag(, 0) changed exception 0x%x", e);
	fh_clearall();
	pc_pass();
}
