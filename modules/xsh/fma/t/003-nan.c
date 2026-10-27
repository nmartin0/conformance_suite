/*
 * fma() with a NaN operand returns a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 2.0, (double)NAN, (double)NAN, 2.0, (double)INFINITY };
	static const double in2[] = { 2.0, (double)NAN, (double)NAN, 2.0, 3.0, 2.0 };
	static const double in3[] = { 3.0, 3.0, 3.0, (double)NAN, (double)NAN, (double)NAN };
	volatile double x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fma(x, y, z);
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g, not a NaN", (double)x, (double)y, (double)z, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 2.0f, (float)NAN, (float)NAN, 2.0f, (float)INFINITY };
	static const float in2[] = { 2.0f, (float)NAN, (float)NAN, 2.0f, 3.0f, 2.0f };
	static const float in3[] = { 3.0f, 3.0f, 3.0f, (float)NAN, (float)NAN, (float)NAN };
	volatile float x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fmaf(x, y, z);
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g, not a NaN", (double)x, (double)y, (double)z, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 2.0L, (long double)NAN, (long double)NAN, 2.0L, (long double)INFINITY };
	static const long double in2[] = { 2.0L, (long double)NAN, (long double)NAN, 2.0L, 3.0L, 2.0L };
	static const long double in3[] = { 3.0L, 3.0L, 3.0L, (long double)NAN, (long double)NAN, (long double)NAN };
	volatile long double x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fmal(x, y, z);
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g, not a NaN", (double)x, (double)y, (double)z, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.fma.003");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
