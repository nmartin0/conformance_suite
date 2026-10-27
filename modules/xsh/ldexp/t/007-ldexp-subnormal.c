/*
 * ldexp() returns a representable subnormal result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldexp.007
 */
#include "mt.h"

static void run_d(void)
{
	volatile double h;

	h = DBL_MIN / 2;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	{
		volatile int n = -1;
		volatile double x = DBL_MIN, r;

		r = ldexp(x, n);
		pc_check(r == h, "ldexp(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

static void run_f(void)
{
	volatile float h;

	h = FLT_MIN / 2;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	{
		volatile int n = -1;
		volatile float x = FLT_MIN, r;

		r = ldexpf(x, n);
		pc_check(r == h, "ldexpf(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

static void run_l(void)
{
	volatile long double h;

	h = LDBL_MIN / 2;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	{
		volatile int n = -1;
		volatile long double x = LDBL_MIN, r;

		r = ldexpl(x, n);
		pc_check(r == h, "ldexpl(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

int main(void)
{
	pc_begin("XSH.ldexp.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
