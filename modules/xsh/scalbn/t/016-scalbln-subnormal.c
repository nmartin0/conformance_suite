/*
 * scalbln() returns a representable subnormal result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.016
 */
#include "mt.h"

static void run_d(void)
{
	volatile double h;

	h = DBL_MIN / FLT_RADIX;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	{
		volatile long n = -1;
		volatile double x = DBL_MIN, r;

		r = scalbln(x, n);
		pc_check(r == h, "scalbln(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

static void run_f(void)
{
	volatile float h;

	h = FLT_MIN / FLT_RADIX;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	{
		volatile long n = -1;
		volatile float x = FLT_MIN, r;

		r = scalblnf(x, n);
		pc_check(r == h, "scalblnf(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

static void run_l(void)
{
	volatile long double h;

	h = LDBL_MIN / FLT_RADIX;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	{
		volatile long n = -1;
		volatile long double x = LDBL_MIN, r;

		r = scalblnl(x, n);
		pc_check(r == h, "scalblnl(MIN, -1) returned %g, expected %g", (double)r, (double)h);
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.016");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
