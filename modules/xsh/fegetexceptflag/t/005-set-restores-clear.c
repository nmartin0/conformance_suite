/*
 * fesetexceptflag() restores a flag that was clear when stored, and raises no other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetexceptflag.005
 */
#include "fh.h"

int main(void)
{
	fexcept_t f;

	pc_begin("XSH.fegetexceptflag.005");
	fh_pair();
	fh_clearall();
	if (fegetexceptflag(&f, fh_e1) != 0)
		pc_unresolved("fegetexceptflag() failed");
	fh_raise(fh_e1);
	pc_test();
	if (fesetexceptflag(&f, fh_e1) != 0)
		pc_fail("fesetexceptflag() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) == 0, "exception 0x%x still set after fesetexceptflag() with a stored clear flag", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) == 0, "fesetexceptflag() raised exception 0x%x", fh_e2);
	fh_clearall();
	pc_pass();
}
