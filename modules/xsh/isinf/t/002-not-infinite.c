/*
 * isinf() is zero for finite values and NaNs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isinf.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0), 1.0, DBL_MAX, (-DBL_MAX), (double)NAN, (-(double)NAN) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r == 0, "isinf(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f), 1.0f, FLT_MAX, (-FLT_MAX), (float)NAN, (-(float)NAN) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r == 0, "isinf(%g) gave %d, expected 0", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L), 1.0L, LDBL_MAX, (-LDBL_MAX), (long double)NAN, (-(long double)NAN) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r == 0, "isinf(%g) gave %d, expected 0", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isinf.002");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
