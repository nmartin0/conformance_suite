/*
 * copysign() treats the sign of a zero as a sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 1.0, -1.0, 0.0, (-0.0), 0.0, (-0.0) };
	static const double in2[] = { (-0.0), 0.0, -1.0, 1.0, 0.0, (-0.0) };
	static const double want[] = { -1.0, 1.0, (-0.0), 0.0, 0.0, (-0.0) };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysign(x, y);
		pc_check(MT_SAME(r, want[i]), "copysign(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 1.0f, -1.0f, 0.0f, (-0.0f), 0.0f, (-0.0f) };
	static const float in2[] = { (-0.0f), 0.0f, -1.0f, 1.0f, 0.0f, (-0.0f) };
	static const float want[] = { -1.0f, 1.0f, (-0.0f), 0.0f, 0.0f, (-0.0f) };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignf(x, y);
		pc_check(MT_SAME(r, want[i]), "copysignf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 1.0L, -1.0L, 0.0L, (-0.0L), 0.0L, (-0.0L) };
	static const long double in2[] = { (-0.0L), 0.0L, -1.0L, 1.0L, 0.0L, (-0.0L) };
	static const long double want[] = { -1.0L, 1.0L, (-0.0L), 0.0L, 0.0L, (-0.0L) };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in1[i];
		y = in2[i];
		r = copysignl(x, y);
		pc_check(MT_SAME(r, want[i]), "copysignl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.copysign.002");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
