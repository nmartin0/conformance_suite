/*
 * fesetenv() reinstates the rounding direction stored by fegetenv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.003
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int m0, m1, got;

	pc_begin("XSH.fegetenv.003");
	fh_two_modes(&m0, &m1);
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	if (fesetround(m1) != 0)
		pc_unresolved("fesetround(%d) failed", m1);
	pc_test();
	if (fesetenv(&env) != 0)
		pc_fail("fesetenv() failed");
	got = fegetround();
	(void)fesetround(m0);
	pc_check(got == m0, "rounding direction is %d after fesetenv(), expected %d", got, m0);
	pc_pass();
}
