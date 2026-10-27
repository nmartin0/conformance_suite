/*
 * fpclassify() puts subnormal values in FP_SUBNORMAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double h;

	h = DBL_MIN / 2;
	if (h == 0 || h >= DBL_MIN)
		pc_notinuse("subnormal values are not supported for double");
	{
		volatile int r;

		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(MIN/2) of type double gave %d, expected FP_SUBNORMAL", r);
		h = -h;
		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(-MIN/2) of type double gave %d, expected FP_SUBNORMAL", r);
	}
}

static void run_f(void)
{
	volatile float h;

	h = FLT_MIN / 2;
	if (h == 0 || h >= FLT_MIN)
		pc_notinuse("subnormal values are not supported for float");
	{
		volatile int r;

		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(MIN/2) of type float gave %d, expected FP_SUBNORMAL", r);
		h = -h;
		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(-MIN/2) of type float gave %d, expected FP_SUBNORMAL", r);
	}
}

static void run_l(void)
{
	volatile long double h;

	h = LDBL_MIN / 2;
	if (h == 0 || h >= LDBL_MIN)
		pc_notinuse("subnormal values are not supported for long double");
	{
		volatile int r;

		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(MIN/2) of type long double gave %d, expected FP_SUBNORMAL", r);
		h = -h;
		r = fpclassify(h);
		pc_check(r == FP_SUBNORMAL, "fpclassify(-MIN/2) of type long double gave %d, expected FP_SUBNORMAL", r);
	}
}

int main(void)
{
	pc_begin("XSH.fpclassify.004");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
