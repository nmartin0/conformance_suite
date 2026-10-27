/*
 * isunordered() is 1 when an operand is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isunordered.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { (double)NAN, 1.0, (double)NAN, (double)NAN, -(double)INFINITY, -(double)NAN };
	static const double ys[] = { 1.0, (double)NAN, (double)NAN, (double)INFINITY, (double)NAN, 0.0 };
	volatile double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 1, "isunordered(%g, %g) of type double gave %d, expected 1", (double)x, (double)y, r);
	}
}

static void run_f(void)
{
	static const float xs[] = { (float)NAN, 1.0f, (float)NAN, (float)NAN, -(float)INFINITY, -(float)NAN };
	static const float ys[] = { 1.0f, (float)NAN, (float)NAN, (float)INFINITY, (float)NAN, 0.0f };
	volatile float x, y;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 1, "isunordered(%g, %g) of type float gave %d, expected 1", (double)x, (double)y, r);
	}
}

static void run_l(void)
{
	static const long double xs[] = { (long double)NAN, 1.0L, (long double)NAN, (long double)NAN, -(long double)INFINITY, -(long double)NAN };
	static const long double ys[] = { 1.0L, (long double)NAN, (long double)NAN, (long double)INFINITY, (long double)NAN, 0.0L };
	volatile long double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = isunordered(x, y);
		pc_check(r == 1, "isunordered(%g, %g) of type long double gave %d, expected 1", (double)x, (double)y, r);
	}
}

int main(void)
{
	pc_begin("XSH.isunordered.001");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
