/*
 * An exception cleared between feholdexcept() and feupdateenv() stays clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.007
 */
#include "fh.h"

int main(void)
{
	fenv_t env;

	pc_begin("XSH.feupdateenv.007");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	if (feholdexcept(&env) != 0)	/* e1 saved, flags cleared */
		pc_unresolved("feholdexcept() did not return zero");
	fh_raise(fh_e2);
	if (feclearexcept(fh_e2) != 0)
		pc_unresolved("feclearexcept() failed");
	pc_test();
	if (feupdateenv(&env) != 0)
		pc_fail("feupdateenv() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) != 0, "exception 0x%x saved by feholdexcept() is not set after feupdateenv()", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) == 0, "exception 0x%x, cleared before feupdateenv(), is set after it", fh_e2);
	fh_clearall();
	pc_pass();
}
