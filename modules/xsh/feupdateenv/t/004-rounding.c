/*
 * feupdateenv() installs the rounding direction of the environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.004
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int m0, m1, got;

	pc_begin("XSH.feupdateenv.004");
	fh_two_modes(&m0, &m1);
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	if (fesetround(m1) != 0)
		pc_unresolved("fesetround(%d) failed", m1);
	pc_test();
	if (feupdateenv(&env) != 0)
		pc_fail("feupdateenv() failed");
	got = fegetround();
	(void)fesetround(m0);
	pc_check(got == m0, "rounding direction is %d after feupdateenv(), expected %d", got, m0);
	pc_pass();
}
