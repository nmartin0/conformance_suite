/*
 * fmax() of two NaNs is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmax.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, (-(double)NAN) };
	static const double in2[] = { (double)NAN, (double)NAN };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = fmax(x, y);
		pc_check(isnan(r), "fmax(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, (-(float)NAN) };
	static const float in2[] = { (float)NAN, (float)NAN };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxf(x, y);
		pc_check(isnan(r), "fmaxf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, (-(long double)NAN) };
	static const long double in2[] = { (long double)NAN, (long double)NAN };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = fmaxl(x, y);
		pc_check(isnan(r), "fmaxl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.fmax.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
