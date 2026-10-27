/*
 * floor() of a zero returns that zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.floor.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0) };
	static const double want[] = { 0.0, (-0.0) };
	volatile double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floor(x);
		pc_check(MT_SAME(r, want[i]), "floor(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f) };
	static const float want[] = { 0.0f, (-0.0f) };
	volatile float x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floorf(x);
		pc_check(MT_SAME(r, want[i]), "floorf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L) };
	static const long double want[] = { 0.0L, (-0.0L) };
	volatile long double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = floorl(x);
		pc_check(MT_SAME(r, want[i]), "floorl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.floor.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
