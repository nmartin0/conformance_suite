/*
 * ldexp() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldexp.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	volatile int n = 3;

	x = (double)NAN;
	r = ldexp(x, n);
	pc_check(isnan(r), "ldexp(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	volatile int n = 3;

	x = (float)NAN;
	r = ldexpf(x, n);
	pc_check(isnan(r), "ldexpf(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	volatile int n = 3;

	x = (long double)NAN;
	r = ldexpl(x, n);
	pc_check(isnan(r), "ldexpl(NaN, 3) returned %g, not a NaN", (double)r);
}

int main(void)
{
	pc_begin("XSH.ldexp.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
