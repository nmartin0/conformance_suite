/*
 * fesetenv(FE_DFL_ENV) succeeds and installs the default rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.006
 */
#include "fh.h"

int main(void)
{
	int m0, m1, got, r;

	pc_begin("XSH.fegetenv.006");
	fh_two_modes(&m0, &m1);	/* m0 is the direction at program start */
	if (fesetround(m1) != 0)
		pc_unresolved("fesetround(%d) failed", m1);
	pc_test();
	r = fesetenv(FE_DFL_ENV);
	pc_check(r == 0, "fesetenv(FE_DFL_ENV) returned %d, expected 0", r);
	got = fegetround();
	(void)fesetround(m0);
	pc_check(got == m0, "rounding direction is %d after fesetenv(FE_DFL_ENV), expected the startup value %d", got, m0);
	pc_pass();
}
