/*
 * round() of an infinity returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.round.006
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
		r = round(x);
		pc_check(MT_SAME(r, want[i]), "round(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
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
		r = roundf(x);
		pc_check(MT_SAME(r, want[i]), "roundf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
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
		r = roundl(x);
		pc_check(MT_SAME(r, want[i]), "roundl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.round.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
