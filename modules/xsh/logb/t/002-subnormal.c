/*
 * logb() treats a subnormal value as normalized.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.002
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	double h;

	h = DBL_MIN / FLT_RADIX;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	x = h;
	r = logb(x);
	pc_check(r == DBL_MIN_EXP - 2, "logb(MIN/radix) returned %g, expected %d", (double)r, DBL_MIN_EXP - 2);
}

static void run_f(void)
{
	volatile float x, r;
	float h;

	h = FLT_MIN / FLT_RADIX;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	x = h;
	r = logbf(x);
	pc_check(r == FLT_MIN_EXP - 2, "logbf(MIN/radix) returned %g, expected %d", (double)r, FLT_MIN_EXP - 2);
}

static void run_l(void)
{
	volatile long double x, r;
	long double h;

	h = LDBL_MIN / FLT_RADIX;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	x = h;
	r = logbl(x);
	pc_check(r == LDBL_MIN_EXP - 2, "logbl(MIN/radix) returned %g, expected %d", (double)r, LDBL_MIN_EXP - 2);
}

int main(void)
{
	pc_begin("XSH.logb.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
