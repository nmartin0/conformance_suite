/*
 * ilogb() treats a subnormal value as normalized.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.003
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile int r;
	double h;

	h = DBL_MIN / FLT_RADIX;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	x = h;
	r = ilogb(x);
	pc_check(r == DBL_MIN_EXP - 2, "ilogb(MIN/radix) returned %d, expected %d", (int)r, DBL_MIN_EXP - 2);
}

static void run_f(void)
{
	volatile float x;
	volatile int r;
	float h;

	h = FLT_MIN / FLT_RADIX;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	x = h;
	r = ilogbf(x);
	pc_check(r == FLT_MIN_EXP - 2, "ilogbf(MIN/radix) returned %d, expected %d", (int)r, FLT_MIN_EXP - 2);
}

static void run_l(void)
{
	volatile long double x;
	volatile int r;
	long double h;

	h = LDBL_MIN / FLT_RADIX;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	x = h;
	r = ilogbl(x);
	pc_check(r == LDBL_MIN_EXP - 2, "ilogbl(MIN/radix) returned %d, expected %d", (int)r, LDBL_MIN_EXP - 2);
}

int main(void)
{
	pc_begin("XSH.ilogb.003");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
