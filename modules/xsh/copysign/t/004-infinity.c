/*
 * copysign() with infinite operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)INFINITY, (-(double)INFINITY), 1.0, 1.0, (double)INFINITY };
	static const double in2[] = { -1.0, 1.0, (-(double)INFINITY), (double)INFINITY, (-(double)INFINITY) };
	static const double want[] = { (-(double)INFINITY), (double)INFINITY, -1.0, 1.0, (-(double)INFINITY) };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in1[i];
		y = in2[i];
		r = copysign(x, y);
		pc_check(MT_SAME(r, want[i]), "copysign(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)INFINITY, (-(float)INFINITY), 1.0f, 1.0f, (float)INFINITY };
	static const float in2[] = { -1.0f, 1.0f, (-(float)INFINITY), (float)INFINITY, (-(float)INFINITY) };
	static const float want[] = { (-(float)INFINITY), (float)INFINITY, -1.0f, 1.0f, (-(float)INFINITY) };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignf(x, y);
		pc_check(MT_SAME(r, want[i]), "copysignf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)INFINITY, (-(long double)INFINITY), 1.0L, 1.0L, (long double)INFINITY };
	static const long double in2[] = { -1.0L, 1.0L, (-(long double)INFINITY), (long double)INFINITY, (-(long double)INFINITY) };
	static const long double want[] = { (-(long double)INFINITY), (long double)INFINITY, -1.0L, 1.0L, (-(long double)INFINITY) };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignl(x, y);
		pc_check(MT_SAME(r, want[i]), "copysignl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.copysign.004");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
