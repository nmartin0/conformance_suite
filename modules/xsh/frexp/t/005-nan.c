/*
 * frexp() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	int e;

	x = (double)NAN;
	r = frexp(x, &e);
	pc_check(isnan(r), "frexp(NaN, &e) returned %g, not a NaN", (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	int e;

	x = (float)NAN;
	r = frexpf(x, &e);
	pc_check(isnan(r), "frexpf(NaN, &e) returned %g, not a NaN", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	int e;

	x = (long double)NAN;
	r = frexpl(x, &e);
	pc_check(isnan(r), "frexpl(NaN, &e) returned %g, not a NaN", (double)r);
}

int main(void)
{
	pc_begin("XSH.frexp.005");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
