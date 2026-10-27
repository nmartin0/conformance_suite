/*
 * fpclassify() puts zeros and normal numbers in FP_ZERO and FP_NORMAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, -0.0, 1.0, -1.0, 0.1, DBL_MIN, -DBL_MIN, DBL_MAX, -DBL_MAX };
	static const int want[] = { FP_ZERO, FP_ZERO, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 9; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == want[i], "fpclassify(%g) of type double gave %d, expected %d", (double)x, r, want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, -0.0f, 1.0f, -1.0f, 0.1f, FLT_MIN, -FLT_MIN, FLT_MAX, -FLT_MAX };
	static const int want[] = { FP_ZERO, FP_ZERO, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 9; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == want[i], "fpclassify(%g) of type float gave %d, expected %d", (double)x, r, want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, -0.0L, 1.0L, -1.0L, 0.1L, LDBL_MIN, -LDBL_MIN, LDBL_MAX, -LDBL_MAX };
	static const int want[] = { FP_ZERO, FP_ZERO, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL, FP_NORMAL };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 9; i++) {
		x = in[i];
		r = fpclassify(x);
		pc_check(r == want[i], "fpclassify(%g) of type long double gave %d, expected %d", (double)x, r, want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fpclassify.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
