/*
 * scalbn() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	volatile int n = 3;

	x = (double)NAN;
	r = scalbn(x, n);
	pc_check(isnan(r), "scalbn(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	volatile int n = 3;

	x = (float)NAN;
	r = scalbnf(x, n);
	pc_check(isnan(r), "scalbnf(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	volatile int n = 3;

	x = (long double)NAN;
	r = scalbnl(x, n);
	pc_check(isnan(r), "scalbnl(NaN, 3) returned %g, not a NaN", (double)r);
}

int main(void)
{
	pc_begin("XSH.scalbn.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
