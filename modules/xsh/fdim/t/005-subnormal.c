/*
 * fdim() returns a subnormal difference exactly.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdim.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, h, r;

	h = DBL_MIN / 2;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	x = 1.5 * DBL_MIN;
	y = DBL_MIN;
	r = fdim(x, y);
	pc_check(r == h, "fdim(1.5*MIN, MIN) returned %g, expected %g", (double)r, (double)h);
}

static void run_f(void)
{
	volatile float x, y, h, r;

	h = FLT_MIN / 2;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	x = 1.5f * FLT_MIN;
	y = FLT_MIN;
	r = fdimf(x, y);
	pc_check(r == h, "fdimf(1.5*MIN, MIN) returned %g, expected %g", (double)r, (double)h);
}

static void run_l(void)
{
	volatile long double x, y, h, r;

	h = LDBL_MIN / 2;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	x = 1.5L * LDBL_MIN;
	y = LDBL_MIN;
	r = fdiml(x, y);
	pc_check(r == h, "fdiml(1.5*MIN, MIN) returned %g, expected %g", (double)r, (double)h);
}

int main(void)
{
	pc_begin("XSH.fdim.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
