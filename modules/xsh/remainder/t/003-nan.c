/*
 * remainder() with a NaN operand returns a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remainder.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 2.0, (double)NAN };
	static const double in2[] = { 2.0, (double)NAN, (double)NAN };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = remainder(x, y);
		pc_check(isnan(r), "remainder(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 2.0f, (float)NAN };
	static const float in2[] = { 2.0f, (float)NAN, (float)NAN };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = remainderf(x, y);
		pc_check(isnan(r), "remainderf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 2.0L, (long double)NAN };
	static const long double in2[] = { 2.0L, (long double)NAN, (long double)NAN };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = remainderl(x, y);
		pc_check(isnan(r), "remainderl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.remainder.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
