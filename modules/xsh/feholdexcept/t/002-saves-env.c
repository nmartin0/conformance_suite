/*
 * feholdexcept() saves the environment, so fesetenv() reinstates its rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feholdexcept.002
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int m0, m1, got;

	pc_begin("XSH.feholdexcept.002");
	fh_two_modes(&m0, &m1);
	if (feholdexcept(&env) != 0)
		pc_unresolved("feholdexcept() did not return zero");
	if (fesetround(m1) != 0)
		pc_unresolved("fesetround(%d) failed", m1);
	pc_test();
	if (fesetenv(&env) != 0)
		pc_fail("fesetenv() failed");
	got = fegetround();
	(void)fesetround(m0);
	pc_check(got == m0, "rounding direction is %d after fesetenv() with the held environment, expected %d", got, m0);
	pc_pass();
}
