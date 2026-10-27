/*
 * isunordered() is 0 for ordered operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isunordered.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { 1.0, 2.0, 1.0, 0.0, (double)INFINITY, -(double)INFINITY, DBL_MAX };
	static const double ys[] = { 2.0, 1.0, 1.0, -0.0, 1.0, (double)INFINITY, -DBL_MAX };
	volatile double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 0, "isunordered(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
	}
}

static void run_f(void)
{
	static const float xs[] = { 1.0f, 2.0f, 1.0f, 0.0f, (float)INFINITY, -(float)INFINITY, FLT_MAX };
	static const float ys[] = { 2.0f, 1.0f, 1.0f, -0.0f, 1.0f, (float)INFINITY, -FLT_MAX };
	volatile float x, y;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 0, "isunordered(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
	}
}

static void run_l(void)
{
	static const long double xs[] = { 1.0L, 2.0L, 1.0L, 0.0L, (long double)INFINITY, -(long double)INFINITY, LDBL_MAX };
	static const long double ys[] = { 2.0L, 1.0L, 1.0L, -0.0L, 1.0L, (long double)INFINITY, -LDBL_MAX };
	volatile long double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 0, "isunordered(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
	}
}

int main(void)
{
	pc_begin("XSH.isunordered.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
