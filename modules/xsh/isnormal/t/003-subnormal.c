/*
 * isnormal() is zero for subnormal values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isnormal.003
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

		r = isnormal(h);
		pc_check(r == 0, "isnormal(MIN/2) of type double gave %d, expected 0", r);
		h = -h;
		r = isnormal(h);
		pc_check(r == 0, "isnormal(-MIN/2) of type double gave %d, expected 0", r);
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

		r = isnormal(h);
		pc_check(r == 0, "isnormal(MIN/2) of type float gave %d, expected 0", r);
		h = -h;
		r = isnormal(h);
		pc_check(r == 0, "isnormal(-MIN/2) of type float gave %d, expected 0", r);
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

		r = isnormal(h);
		pc_check(r == 0, "isnormal(MIN/2) of type long double gave %d, expected 0", r);
		h = -h;
		r = isnormal(h);
		pc_check(r == 0, "isnormal(-MIN/2) of type long double gave %d, expected 0", r);
	}
}

int main(void)
{
	pc_begin("XSH.isnormal.003");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
