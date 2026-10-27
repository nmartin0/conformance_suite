/*
 * ilogb() of a zero returns FP_ILOGB0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, -0.0 };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogb(x);
		pc_check(r == FP_ILOGB0, "ilogb(%g) returned %d, expected FP_ILOGB0", (double)x, (int)r);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, -0.0f };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogbf(x);
		pc_check(r == FP_ILOGB0, "ilogbf(%g) returned %d, expected FP_ILOGB0", (double)x, (int)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, -0.0L };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogbl(x);
		pc_check(r == FP_ILOGB0, "ilogbl(%g) returned %d, expected FP_ILOGB0", (double)x, (int)r);
	}
}

int main(void)
{
	pc_begin("XSH.ilogb.004");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
