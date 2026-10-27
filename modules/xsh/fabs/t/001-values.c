/*
 * fabs() returns the magnitude of x.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fabs.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -3.5, 3.5, -0.25, 0.25, -100.0625, DBL_MAX, (-DBL_MAX) };
	static const double want[] = { 3.5, 3.5, 0.25, 0.25, 100.0625, DBL_MAX, DBL_MAX };
	volatile double x, r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = fabs(x);
		pc_check(r == want[i], "fabs(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { -3.5f, 3.5f, -0.25f, 0.25f, -100.0625f, FLT_MAX, (-FLT_MAX) };
	static const float want[] = { 3.5f, 3.5f, 0.25f, 0.25f, 100.0625f, FLT_MAX, FLT_MAX };
	volatile float x, r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = fabsf(x);
		pc_check(r == want[i], "fabsf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { -3.5L, 3.5L, -0.25L, 0.25L, -100.0625L, LDBL_MAX, (-LDBL_MAX) };
	static const long double want[] = { 3.5L, 3.5L, 0.25L, 0.25L, 100.0625L, LDBL_MAX, LDBL_MAX };
	volatile long double x, r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = fabsl(x);
		pc_check(r == want[i], "fabsl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fabs.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
