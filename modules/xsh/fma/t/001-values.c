/*
 * fma() computes x*y + z.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 2.0, -2.0, 0.5, 1.5, 0.0, 3.0 };
	static const double in2[] = { 3.0, 3.0, 0.5, 2.0, 5.0, 0.25 };
	static const double in3[] = { 4.0, 1.0, 0.25, -3.0, 2.0, 0.25 };
	static const double want[] = { 10.0, -5.0, 0.5, 0.0, 2.0, 1.0 };
	volatile double x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fma(x, y, z);
		pc_check(r == want[i], "fma(%g, %g, %g) returned %g, expected %g", (double)x, (double)y, (double)z, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 2.0f, -2.0f, 0.5f, 1.5f, 0.0f, 3.0f };
	static const float in2[] = { 3.0f, 3.0f, 0.5f, 2.0f, 5.0f, 0.25f };
	static const float in3[] = { 4.0f, 1.0f, 0.25f, -3.0f, 2.0f, 0.25f };
	static const float want[] = { 10.0f, -5.0f, 0.5f, 0.0f, 2.0f, 1.0f };
	volatile float x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fmaf(x, y, z);
		pc_check(r == want[i], "fmaf(%g, %g, %g) returned %g, expected %g", (double)x, (double)y, (double)z, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 2.0L, -2.0L, 0.5L, 1.5L, 0.0L, 3.0L };
	static const long double in2[] = { 3.0L, 3.0L, 0.5L, 2.0L, 5.0L, 0.25L };
	static const long double in3[] = { 4.0L, 1.0L, 0.25L, -3.0L, 2.0L, 0.25L };
	static const long double want[] = { 10.0L, -5.0L, 0.5L, 0.0L, 2.0L, 1.0L };
	volatile long double x, y, z, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		z = in3[i];
		r = fmal(x, y, z);
		pc_check(r == want[i], "fmal(%g, %g, %g) returned %g, expected %g", (double)x, (double)y, (double)z, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fma.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
