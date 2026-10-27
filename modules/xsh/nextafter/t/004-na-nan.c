/*
 * nextafter() with a NaN operand returns a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 1.0, (double)NAN };
	static const double in2[] = { 1.0, (double)NAN, (double)NAN };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafter(x, y);
		pc_check(isnan(r), "nextafter(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 1.0f, (float)NAN };
	static const float in2[] = { 1.0f, (float)NAN, (float)NAN };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterf(x, y);
		pc_check(isnan(r), "nextafterf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 1.0L, (long double)NAN };
	static const long double in2[] = { 1.0L, (long double)NAN, (long double)NAN };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterl(x, y);
		pc_check(isnan(r), "nextafterl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
