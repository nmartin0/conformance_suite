/*
 * floor() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.floor.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)NAN, (-(double)NAN) };
	volatile double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floor(x);
		pc_check(isnan(r), "floor(NaN) returned %g, not a NaN", (double)r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)NAN, (-(float)NAN) };
	volatile float x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floorf(x);
		pc_check(isnan(r), "floorf(NaN) returned %g, not a NaN", (double)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)NAN, (-(long double)NAN) };
	volatile long double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floorl(x);
		pc_check(isnan(r), "floorl(NaN) returned %g, not a NaN", (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.floor.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
