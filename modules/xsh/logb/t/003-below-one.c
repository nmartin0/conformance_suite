/*
 * logb() of a number below one follows the inequality on the page.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.003
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;

	x = 0.375;
	r = logb(x);
	pc_check(r == -2, "logb(0.375) returned %g, expected -2", (double)r);
}

static void run_f(void)
{
	volatile float x, r;

	x = 0.375f;
	r = logbf(x);
	pc_check(r == -2, "logbf(0.375) returned %g, expected -2", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;

	x = 0.375L;
	r = logbl(x);
	pc_check(r == -2, "logbl(0.375) returned %g, expected -2", (double)r);
}

int main(void)
{
	pc_begin("XSH.logb.003");
	if (FLT_RADIX != 2)
		pc_notinuse("the radix is not 2");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
