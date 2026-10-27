/*
 * fesetexceptflag() changes only the flags named in its argument.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.006
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;

	pc_begin("XSH.fegetexceptflag.006");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	if (fegetexceptflag(&f, fh_e1 | fh_e2) != 0)
		pc_unresolved("fegetexceptflag() failed");
	fh_clearall();
	fh_raise(fh_e2);
	pc_test();
	if (fesetexceptflag(&f, fh_e1) != 0)
		pc_fail("fesetexceptflag() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) != 0, "exception 0x%x not set by fesetexceptflag()", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) != 0, "exception 0x%x was changed by fesetexceptflag(, 0x%x)", fh_e2, fh_e1);
	fh_clearall();
	pc_pass();
}
