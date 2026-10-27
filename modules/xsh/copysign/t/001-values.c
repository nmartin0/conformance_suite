/*
 * copysign() gives the magnitude of x and the sign of y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 3.0, -3.0, 3.0, -3.0, 2.5, 0.25 };
	static const double in2[] = { -1.0, 1.0, 1.0, -1.0, -7.0, 2.0 };
	static const double want[] = { -3.0, 3.0, 3.0, -3.0, -2.5, 0.25 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysign(x, y);
		pc_check(r == want[i], "copysign(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 3.0f, -3.0f, 3.0f, -3.0f, 2.5f, 0.25f };
	static const float in2[] = { -1.0f, 1.0f, 1.0f, -1.0f, -7.0f, 2.0f };
	static const float want[] = { -3.0f, 3.0f, 3.0f, -3.0f, -2.5f, 0.25f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignf(x, y);
		pc_check(r == want[i], "copysignf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 3.0L, -3.0L, 3.0L, -3.0L, 2.5L, 0.25L };
	static const long double in2[] = { -1.0L, 1.0L, 1.0L, -1.0L, -7.0L, 2.0L };
	static const long double want[] = { -3.0L, 3.0L, 3.0L, -3.0L, -2.5L, 0.25L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignl(x, y);
		pc_check(r == want[i], "copysignl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.copysign.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
