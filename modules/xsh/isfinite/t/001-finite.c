/*
 * isfinite() is non-zero for finite values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isfinite.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, -0.0, 1.0, -1.0, 0.1, DBL_MIN, DBL_MAX, DBL_MIN / 2 };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isfinite(x);
		pc_check(r != 0, "isfinite(%g) of type double gave 0", (double)x);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, -0.0f, 1.0f, -1.0f, 0.1f, FLT_MIN, FLT_MAX, FLT_MIN / 2 };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isfinite(x);
		pc_check(r != 0, "isfinite(%g) of type float gave 0", (double)x);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, -0.0L, 1.0L, -1.0L, 0.1L, LDBL_MIN, LDBL_MAX, LDBL_MIN / 2 };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		r = isfinite(x);
		pc_check(r != 0, "isfinite(%g) of type long double gave 0", (double)x);
	}
}

int main(void)
{
	pc_begin("XSH.isfinite.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
