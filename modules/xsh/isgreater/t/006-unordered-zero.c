/*
 * The five comparison macros are 0 when an operand is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isgreater.006
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { (double)NAN, 1.0, (double)NAN, (double)NAN, -(double)INFINITY };
	static const double ys[] = { 1.0, (double)NAN, (double)NAN, (double)INFINITY, (double)NAN };
	volatile double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == 0, "isgreater(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
		r = isgreaterequal(x, y);
		pc_check(r == 0, "isgreaterequal(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
		r = isless(x, y);
		pc_check(r == 0, "isless(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
		r = islessequal(x, y);
		pc_check(r == 0, "islessequal(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
		r = islessgreater(x, y);
		pc_check(r == 0, "islessgreater(%g, %g) of type double gave %d, expected 0", (double)x, (double)y, r);
	}
}

static void run_f(void)
{
	static const float xs[] = { (float)NAN, 1.0f, (float)NAN, (float)NAN, -(float)INFINITY };
	static const float ys[] = { 1.0f, (float)NAN, (float)NAN, (float)INFINITY, (float)NAN };
	volatile float x, y;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == 0, "isgreater(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
		r = isgreaterequal(x, y);
		pc_check(r == 0, "isgreaterequal(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
		r = isless(x, y);
		pc_check(r == 0, "isless(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
		r = islessequal(x, y);
		pc_check(r == 0, "islessequal(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
		r = islessgreater(x, y);
		pc_check(r == 0, "islessgreater(%g, %g) of type float gave %d, expected 0", (double)x, (double)y, r);
	}
}

static void run_l(void)
{
	static const long double xs[] = { (long double)NAN, 1.0L, (long double)NAN, (long double)NAN, -(long double)INFINITY };
	static const long double ys[] = { 1.0L, (long double)NAN, (long double)NAN, (long double)INFINITY, (long double)NAN };
	volatile long double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = xs[i];
		y = ys[i];
		r = isgreater(x, y);
		pc_check(r == 0, "isgreater(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
		r = isgreaterequal(x, y);
		pc_check(r == 0, "isgreaterequal(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
		r = isless(x, y);
		pc_check(r == 0, "isless(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
		r = islessequal(x, y);
		pc_check(r == 0, "islessequal(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
		r = islessgreater(x, y);
		pc_check(r == 0, "islessgreater(%g, %g) of type long double gave %d, expected 0", (double)x, (double)y, r);
	}
}

int main(void)
{
	pc_begin("XSH.isgreater.006");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
