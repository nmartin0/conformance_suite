/*
 * fegetexceptflag() does not change the exception flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.002
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;

	pc_begin("XSH.fegetexceptflag.002");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	pc_test();
	if (fegetexceptflag(&f, fh_e1 | fh_e2) != 0)
		pc_unresolved("fegetexceptflag() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) != 0, "exception 0x%x was cleared by fegetexceptflag()", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) == 0, "exception 0x%x was raised by fegetexceptflag()", fh_e2);
	fh_clearall();
	pc_pass();
}
