/*
 * ilogb() of a NaN returns FP_ILOGBNAN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile int r;

	x = (double)NAN;
	r = ilogb(x);
	pc_check(r == FP_ILOGBNAN, "ilogb(NaN) returned %d, expected FP_ILOGBNAN", (int)r);
}

static void run_f(void)
{
	volatile float x;
	volatile int r;

	x = (float)NAN;
	r = ilogbf(x);
	pc_check(r == FP_ILOGBNAN, "ilogbf(NaN) returned %d, expected FP_ILOGBNAN", (int)r);
}

static void run_l(void)
{
	volatile long double x;
	volatile int r;

	x = (long double)NAN;
	r = ilogbl(x);
	pc_check(r == FP_ILOGBNAN, "ilogbl(NaN) returned %d, expected FP_ILOGBNAN", (int)r);
}

int main(void)
{
	pc_begin("XSH.ilogb.006");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
