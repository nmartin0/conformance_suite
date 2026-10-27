/*
 * fdim() of x not greater than y is +0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdim.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 3.0, 3.0, -2.0, -5.0, 0.0, (-0.0) };
	static const double in2[] = { 5.0, 3.0, -2.0, 2.0, (-0.0), 0.0 };
	static const double want[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fdim(x, y);
		pc_check(MT_SAME(r, want[i]), "fdim(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 3.0f, 3.0f, -2.0f, -5.0f, 0.0f, (-0.0f) };
	static const float in2[] = { 5.0f, 3.0f, -2.0f, 2.0f, (-0.0f), 0.0f };
	static const float want[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fdimf(x, y);
		pc_check(MT_SAME(r, want[i]), "fdimf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 3.0L, 3.0L, -2.0L, -5.0L, 0.0L, (-0.0L) };
	static const long double in2[] = { 5.0L, 3.0L, -2.0L, 2.0L, (-0.0L), 0.0L };
	static const long double want[] = { 0.0L, 0.0L, 0.0L, 0.0L, 0.0L, 0.0L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = fdiml(x, y);
		pc_check(MT_SAME(r, want[i]), "fdiml(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fdim.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
