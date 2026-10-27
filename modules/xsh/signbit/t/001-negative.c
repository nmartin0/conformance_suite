/*
 * signbit() is non-zero for negative values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.signbit.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -1.0, -0.1, (-DBL_MIN), (-DBL_MAX), (-(double)INFINITY) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { -1.0f, -0.1f, (-FLT_MIN), (-FLT_MAX), (-(float)INFINITY) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { -1.0L, -0.1L, (-LDBL_MIN), (-LDBL_MAX), (-(long double)INFINITY) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 5; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.signbit.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
