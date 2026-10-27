/*
 * scalbln() of a NaN is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.013
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	volatile long n = 3;

	x = (double)NAN;
	r = scalbln(x, n);
	pc_check(isnan(r), "scalbln(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	volatile long n = 3;

	x = (float)NAN;
	r = scalblnf(x, n);
	pc_check(isnan(r), "scalblnf(NaN, 3) returned %g, not a NaN", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	volatile long n = 3;

	x = (long double)NAN;
	r = scalblnl(x, n);
	pc_check(isnan(r), "scalblnl(NaN, 3) returned %g, not a NaN", (double)r);
}

int main(void)
{
	pc_begin("XSH.scalbn.013");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
