/*
 * remquo() with a NaN operand returns a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remquo.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 2.0, (double)NAN };
	static const double in2[] = { 2.0, (double)NAN, (double)NAN };
	volatile double x, y, r;
	int i, q;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		r = remquo(x, y, &q);
		pc_check(isnan(r), "remquo(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 2.0f, (float)NAN };
	static const float in2[] = { 2.0f, (float)NAN, (float)NAN };
	volatile float x, y, r;
	int i, q;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		r = remquof(x, y, &q);
		pc_check(isnan(r), "remquof(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 2.0L, (long double)NAN };
	static const long double in2[] = { 2.0L, (long double)NAN, (long double)NAN };
	volatile long double x, y, r;
	int i, q;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		r = remquol(x, y, &q);
		pc_check(isnan(r), "remquol(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.remquo.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
