/*
 * Arithmetic rounds according to the direction set by fesetround().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.006
 */
#include "fh.h"

#if defined(FE_UPWARD) && defined(FE_DOWNWARD)
static double third(int mode)
{
	volatile double a = 1.0, b = 3.0, r;

	if (fesetround(mode) != 0)
		pc_unresolved("fesetround(%d) failed", mode);
	r = a / b;
	return r;
}
#endif

int main(void)
{
#if defined(FE_UPWARD) && defined(FE_DOWNWARD)
	double up, down;
	int m0;

	pc_begin("XSH.fegetround.006");
	m0 = fegetround();
	pc_test();
	up = third(FE_UPWARD);
	down = third(FE_DOWNWARD);
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_check(up > down, "1/3 rounded upward (%.17g) is not above 1/3 rounded downward (%.17g)", up, down);
	pc_pass();
#else
	pc_begin("XSH.fegetround.006");
	pc_notinuse("FE_UPWARD and FE_DOWNWARD are not both defined");
#endif
}
