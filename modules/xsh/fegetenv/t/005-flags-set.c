/*
 * fesetenv() installs stored exception flags and raises no others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.005
 */
#include "fh.h"

int main(void)
{
	fenv_t env;

	pc_begin("XSH.fegetenv.005");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	fh_clearall();
	pc_test();
	if (fesetenv(&env) != 0)
		pc_fail("fesetenv() failed");
	pc_check((fetestexcept(fh_e1) & fh_e1) != 0, "exception 0x%x stored as set is not set after fesetenv()", fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) == 0, "fesetenv() raised exception 0x%x", fh_e2);
	fh_clearall();
	pc_pass();
}
