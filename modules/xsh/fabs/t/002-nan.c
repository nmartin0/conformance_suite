/*
 * fabs() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fabs.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)NAN, (-(double)NAN) };
	volatile double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabs(x);
		pc_check(isnan(r), "fabs(NaN) returned %g, not a NaN", (double)r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)NAN, (-(float)NAN) };
	volatile float x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabsf(x);
		pc_check(isnan(r), "fabsf(NaN) returned %g, not a NaN", (double)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)NAN, (-(long double)NAN) };
	volatile long double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabsl(x);
		pc_check(isnan(r), "fabsl(NaN) returned %g, not a NaN", (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.fabs.002");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
