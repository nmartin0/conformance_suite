/*
 * isgreater() compares as the operator does for ordered values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isgreater.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { 2.0, 1.0, 1.0, -1.0, -2.0, 0.0, (-0.0), (double)INFINITY, 1.0, (-(double)INFINITY), (double)INFINITY, (-(double)INFINITY), 0.5 };
	static const double ys[] = { 1.0, 2.0, 1.0, -2.0, -1.0, (-0.0), 0.0, 1.0, (double)INFINITY, 1.0, (double)INFINITY, (-(double)INFINITY), -0.5 };
	static const int want[] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	volatile double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 13; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == want[i], "isgreater(%g, %g) of type double gave %d, expected %d", (double)x, (double)y, r, want[i]);
	}
}

static void run_f(void)
{
	static const float xs[] = { 2.0f, 1.0f, 1.0f, -1.0f, -2.0f, 0.0f, (-0.0f), (float)INFINITY, 1.0f, (-(float)INFINITY), (float)INFINITY, (-(float)INFINITY), 0.5f };
	static const float ys[] = { 1.0f, 2.0f, 1.0f, -2.0f, -1.0f, (-0.0f), 0.0f, 1.0f, (float)INFINITY, 1.0f, (float)INFINITY, (-(float)INFINITY), -0.5f };
	static const int want[] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	volatile float x, y;
	volatile int r;
	int i;

	for (i = 0; i < 13; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == want[i], "isgreater(%g, %g) of type float gave %d, expected %d", (double)x, (double)y, r, want[i]);
	}
}

static void run_l(void)
{
	static const long double xs[] = { 2.0L, 1.0L, 1.0L, -1.0L, -2.0L, 0.0L, (-0.0L), (long double)INFINITY, 1.0L, (-(long double)INFINITY), (long double)INFINITY, (-(long double)INFINITY), 0.5L };
	static const long double ys[] = { 1.0L, 2.0L, 1.0L, -2.0L, -1.0L, (-0.0L), 0.0L, 1.0L, (long double)INFINITY, 1.0L, (long double)INFINITY, (-(long double)INFINITY), -0.5L };
	static const int want[] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	volatile long double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 13; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == want[i], "isgreater(%g, %g) of type long double gave %d, expected %d", (double)x, (double)y, r, want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.isgreater.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
