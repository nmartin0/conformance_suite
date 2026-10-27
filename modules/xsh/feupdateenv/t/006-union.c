/*
 * feupdateenv() keeps flags stored in the environment and adds those raised at the call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.006
 */
#include "fh.h"

int main(void)
{
	fenv_t env;

	pc_begin("XSH.feupdateenv.006");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	if (fegetenv(&env) != 0)	/* stored with e1 set */
		pc_unresolved("fegetenv() failed");
	fh_clearall();
	fh_raise(fh_e2);
	pc_test();
	if (feupdateenv(&env) != 0)
		pc_fail("feupdateenv() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) != 0, "exception 0x%x stored in the environment is not set after feupdateenv()", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) != 0, "exception 0x%x raised before feupdateenv() is not set after it", fh_e2);
	fh_clearall();
	pc_pass();
}
