/*
 * isnormal() is zero for zeros, infinities and NaNs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isnormal.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0), (double)INFINITY, (-(double)INFINITY), (double)NAN, (-(double)NAN) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r == 0, "isnormal(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f), (float)INFINITY, (-(float)INFINITY), (float)NAN, (-(float)NAN) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r == 0, "isnormal(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L), (long double)INFINITY, (-(long double)INFINITY), (long double)NAN, (-(long double)NAN) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r == 0, "isnormal(%g) gave %d, expected 0", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isnormal.002");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
