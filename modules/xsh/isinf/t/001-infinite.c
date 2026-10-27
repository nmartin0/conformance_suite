/*
 * isinf() is non-zero for infinities.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isinf.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, (-(double)INFINITY) };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r != 0, "isinf(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)INFINITY, (-(float)INFINITY) };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r != 0, "isinf(%g) gave %d, expected non-zero", (double)x, r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)INFINITY, (-(long double)INFINITY) };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = isinf(x);
		pc_check(r != 0, "isinf(%g) gave %d, expected non-zero", (double)x, r);
	}
}

int main(void)
{
	pc_begin("XSH.isinf.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
