/*
 * isnan() is non-zero for NaNs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isnan.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)NAN, (-(double)NAN) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r != 0, "isnan(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)NAN, (-(float)NAN) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r != 0, "isnan(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)NAN, (-(long double)NAN) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isnan(x);
		pc_check(r != 0, "isnan(%g) gave %d, expected non-zero", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isnan.001");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
