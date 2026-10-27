/*
 * fmax() treats a lone NaN as missing data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmax.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 3.0, (double)NAN, -3.0, (double)NAN, (-(double)INFINITY) };
	static const double in2[] = { 3.0, (double)NAN, -3.0, (double)NAN, (double)INFINITY, (double)NAN };
	static const double want[] = { 3.0, 3.0, -3.0, -3.0, (double)INFINITY, (-(double)INFINITY) };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fmax(x, y);
		pc_check(r == want[i], "fmax(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 3.0f, (float)NAN, -3.0f, (float)NAN, (-(float)INFINITY) };
	static const float in2[] = { 3.0f, (float)NAN, -3.0f, (float)NAN, (float)INFINITY, (float)NAN };
	static const float want[] = { 3.0f, 3.0f, -3.0f, -3.0f, (float)INFINITY, (-(float)INFINITY) };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxf(x, y);
		pc_check(r == want[i], "fmaxf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 3.0L, (long double)NAN, -3.0L, (long double)NAN, (-(long double)INFINITY) };
	static const long double in2[] = { 3.0L, (long double)NAN, -3.0L, (long double)NAN, (long double)INFINITY, (long double)NAN };
	static const long double want[] = { 3.0L, 3.0L, -3.0L, -3.0L, (long double)INFINITY, (-(long double)INFINITY) };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxl(x, y);
		pc_check(r == want[i], "fmaxl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fmax.002");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
