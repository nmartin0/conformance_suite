/*
 * isnan() is zero for numbers and infinities.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isnan.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0), 1.0, -1.0, DBL_MIN, DBL_MAX, (double)INFINITY, (-(double)INFINITY) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r == 0, "isnan(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f), 1.0f, -1.0f, FLT_MIN, FLT_MAX, (float)INFINITY, (-(float)INFINITY) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r == 0, "isnan(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L), 1.0L, -1.0L, LDBL_MIN, LDBL_MAX, (long double)INFINITY, (-(long double)INFINITY) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r == 0, "isnan(%g) gave %d, expected 0", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isnan.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
