/*
 * isnormal() is non-zero for normal numbers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isnormal.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 1.0, -1.0, 0.1, DBL_MIN, (-DBL_MIN), DBL_MAX, (-DBL_MAX) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r != 0, "isnormal(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { 1.0f, -1.0f, 0.1f, FLT_MIN, (-FLT_MIN), FLT_MAX, (-FLT_MAX) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r != 0, "isnormal(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 1.0L, -1.0L, 0.1L, LDBL_MIN, (-LDBL_MIN), LDBL_MAX, (-LDBL_MAX) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		r = isnormal(x);
		pc_check(r != 0, "isnormal(%g) gave %d, expected non-zero", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isnormal.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
