/*
 * fpclassify() puts NaNs in FP_NAN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)NAN, (-(double)NAN) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_NAN, "fpclassify(%g) gave %d, expected FP_NAN", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)NAN, (-(float)NAN) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_NAN, "fpclassify(%g) gave %d, expected FP_NAN", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)NAN, (-(long double)NAN) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == FP_NAN, "fpclassify(%g) gave %d, expected FP_NAN", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.fpclassify.003");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
