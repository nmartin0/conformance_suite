/*
 * logb() of a zero returns minus HUGE_VAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0) };
	static const double want[] = { (-HUGE_VAL), (-HUGE_VAL) };
	volatile double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = logb(x);
		pc_check(r == want[i], "logb(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f) };
	static const float want[] = { (-HUGE_VALF), (-HUGE_VALF) };
	volatile float x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = logbf(x);
		pc_check(r == want[i], "logbf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L) };
	static const long double want[] = { (-HUGE_VALL), (-HUGE_VALL) };
	volatile long double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = logbl(x);
		pc_check(r == want[i], "logbl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.logb.004");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
