/*
 * nearbyint() of a zero returns that zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nearbyint.006
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
		r = nearbyint(x);
		pc_check(MT_SAME(r, want[i]), "nearbyint(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
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
		r = nearbyintf(x);
		pc_check(MT_SAME(r, want[i]), "nearbyintf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
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
		r = nearbyintl(x);
		pc_check(MT_SAME(r, want[i]), "nearbyintl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.nearbyint.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
