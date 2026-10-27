/*
 * signbit() sees the sign of a zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.signbit.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (-0.0) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 1; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { (-0.0f) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 1; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (-0.0L) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 1; i++) {
		x = in[i];
		r = signbit(x);
		pc_check(r != 0, "signbit(%g) gave %d, expected non-zero", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.signbit.003");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
