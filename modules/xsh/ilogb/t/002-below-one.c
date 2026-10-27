/*
 * ilogb() of a number below one follows the inequality on the logb page.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.002
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile int r;

	x = 0.375;
	r = ilogb(x);
	pc_check(r == -2, "ilogb(0.375) returned %d, expected -2", (int)r);
}

static void run_f(void)
{
	volatile float x;
	volatile int r;

	x = 0.375f;
	r = ilogbf(x);
	pc_check(r == -2, "ilogbf(0.375) returned %d, expected -2", (int)r);
}

static void run_l(void)
{
	volatile long double x;
	volatile int r;

	x = 0.375L;
	r = ilogbl(x);
	pc_check(r == -2, "ilogbl(0.375) returned %d, expected -2", (int)r);
}

int main(void)
{
	pc_begin("XSH.ilogb.002");
	if (FLT_RADIX != 2)
		pc_notinuse("the radix is not 2");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
