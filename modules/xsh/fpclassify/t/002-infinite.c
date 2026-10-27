/*
 * fpclassify() puts infinities in FP_INFINITE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, (-(double)INFINITY) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_INFINITE, "fpclassify(%g) gave %d, expected FP_INFINITE", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)INFINITY, (-(float)INFINITY) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_INFINITE, "fpclassify(%g) gave %d, expected FP_INFINITE", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)INFINITY, (-(long double)INFINITY) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_INFINITE, "fpclassify(%g) gave %d, expected FP_INFINITE", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.fpclassify.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
