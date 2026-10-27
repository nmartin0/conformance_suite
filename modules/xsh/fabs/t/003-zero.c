/*
 * fabs() of a zero is +0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fabs.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0) };
	static const double want[] = { 0.0, 0.0 };
	volatile double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabs(x);
		pc_check(MT_SAME(r, want[i]), "fabs(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f) };
	static const float want[] = { 0.0f, 0.0f };
	volatile float x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabsf(x);
		pc_check(MT_SAME(r, want[i]), "fabsf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L) };
	static const long double want[] = { 0.0L, 0.0L };
	volatile long double x, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = fabsl(x);
		pc_check(MT_SAME(r, want[i]), "fabsl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fabs.003");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
