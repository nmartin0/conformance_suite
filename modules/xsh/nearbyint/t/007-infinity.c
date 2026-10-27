/*
 * nearbyint() of an infinity returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nearbyint.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, (-(double)INFINITY) };
	static const double want[] = { (double)INFINITY, (-(double)INFINITY) };
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
	static const float in[] = { (float)INFINITY, (-(float)INFINITY) };
	static const float want[] = { (float)INFINITY, (-(float)INFINITY) };
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
	static const long double in[] = { (long double)INFINITY, (-(long double)INFINITY) };
	static const long double want[] = { (long double)INFINITY, (-(long double)INFINITY) };
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
	pc_begin("XSH.nearbyint.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
