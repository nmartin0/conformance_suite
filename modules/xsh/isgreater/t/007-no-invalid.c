/*
 * The comparison macros do not raise the invalid exception for unordered operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isgreater.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { (double)NAN, 1.0 };
	static const double ys[] = { 1.0, (double)NAN };
	volatile double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = xs[i];
		y = ys[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = isgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreater(%g, %g) of type double raised the invalid exception", (double)x, (double)y);
		r = isgreaterequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreaterequal(%g, %g) of type double raised the invalid exception", (double)x, (double)y);
		r = isless(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isless(%g, %g) of type double raised the invalid exception", (double)x, (double)y);
		r = islessequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessequal(%g, %g) of type double raised the invalid exception", (double)x, (double)y);
		r = islessgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessgreater(%g, %g) of type double raised the invalid exception", (double)x, (double)y);
	}
	(void)r;
}

static void run_f(void)
{
	static const float xs[] = { (float)NAN, 1.0f };
	static const float ys[] = { 1.0f, (float)NAN };
	volatile float x, y;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = xs[i];
		y = ys[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = isgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreater(%g, %g) of type float raised the invalid exception", (double)x, (double)y);
		r = isgreaterequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreaterequal(%g, %g) of type float raised the invalid exception", (double)x, (double)y);
		r = isless(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isless(%g, %g) of type float raised the invalid exception", (double)x, (double)y);
		r = islessequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessequal(%g, %g) of type float raised the invalid exception", (double)x, (double)y);
		r = islessgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessgreater(%g, %g) of type float raised the invalid exception", (double)x, (double)y);
	}
	(void)r;
}

static void run_l(void)
{
	static const long double xs[] = { (long double)NAN, 1.0L };
	static const long double ys[] = { 1.0L, (long double)NAN };
	volatile long double x, y;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = xs[i];
		y = ys[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = isgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreater(%g, %g) of type long double raised the invalid exception", (double)x, (double)y);
		r = isgreaterequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isgreaterequal(%g, %g) of type long double raised the invalid exception", (double)x, (double)y);
		r = isless(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "isless(%g, %g) of type long double raised the invalid exception", (double)x, (double)y);
		r = islessequal(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessequal(%g, %g) of type long double raised the invalid exception", (double)x, (double)y);
		r = islessgreater(x, y);
		pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0, "islessgreater(%g, %g) of type long double raised the invalid exception", (double)x, (double)y);
	}
	(void)r;
}

int main(void)
{
	pc_begin("XSH.isgreater.007");
	MT_NEED_NAN();
	if (FE_INVALID == 0)
		pc_notinuse("the invalid exception is not supported");
	if (feraiseexcept(FE_INVALID) != 0 || (fetestexcept(FE_INVALID) & FE_INVALID) == 0)
		pc_unresolved("the invalid flag cannot be raised and tested");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
