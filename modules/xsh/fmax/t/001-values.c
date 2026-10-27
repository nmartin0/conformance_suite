/*
 * fmax() returns the maximum of two numbers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmax.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 3.0, 5.0, -3.0, -3.0, 2.5, 0.25, (double)INFINITY, (-(double)INFINITY) };
	static const double in2[] = { 5.0, 3.0, -5.0, 5.0, 2.5, -100.0, 1.0, 1.0 };
	static const double want[] = { 5.0, 5.0, -3.0, 5.0, 2.5, 0.25, (double)INFINITY, 1.0 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in1[i];
		y = in2[i];
		r = fmax(x, y);
		pc_check(r == want[i], "fmax(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 3.0f, 5.0f, -3.0f, -3.0f, 2.5f, 0.25f, (float)INFINITY, (-(float)INFINITY) };
	static const float in2[] = { 5.0f, 3.0f, -5.0f, 5.0f, 2.5f, -100.0f, 1.0f, 1.0f };
	static const float want[] = { 5.0f, 5.0f, -3.0f, 5.0f, 2.5f, 0.25f, (float)INFINITY, 1.0f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxf(x, y);
		pc_check(r == want[i], "fmaxf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 3.0L, 5.0L, -3.0L, -3.0L, 2.5L, 0.25L, (long double)INFINITY, (-(long double)INFINITY) };
	static const long double in2[] = { 5.0L, 3.0L, -5.0L, 5.0L, 2.5L, -100.0L, 1.0L, 1.0L };
	static const long double want[] = { 5.0L, 5.0L, -3.0L, 5.0L, 2.5L, 0.25L, (long double)INFINITY, 1.0L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxl(x, y);
		pc_check(r == want[i], "fmaxl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fmax.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
