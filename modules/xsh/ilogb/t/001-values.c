/*
 * ilogb() returns the exponent of x as an int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double ms[] = { 1.0, 1.5, -1.0, -1.5, 1.0, 1.0, 1.5, -1.0 };
	static const int ks[] = { 0, 1, 3, 3, -2, -7, 5, -3 };
	volatile double x;
	volatile int r;
	double p;
	int i, k;

	for (i = 0; i < 8; i++) {
		p = 1;
		for (k = 0; k < (ks[i] < 0 ? -ks[i] : ks[i]); k++)
			p *= FLT_RADIX;
		x = ks[i] >= 0 ? ms[i] * p : ms[i] / p;
		r = ilogb(x);
		pc_check(r == ks[i], "ilogb(%g) returned %g, expected %d", (double)x, (double)r, ks[i]);
	}
}

static void run_f(void)
{
	static const float ms[] = { 1.0f, 1.5f, -1.0f, -1.5f, 1.0f, 1.0f, 1.5f, -1.0f };
	static const int ks[] = { 0, 1, 3, 3, -2, -7, 5, -3 };
	volatile float x;
	volatile int r;
	float p;
	int i, k;

	for (i = 0; i < 8; i++) {
		p = 1;
		for (k = 0; k < (ks[i] < 0 ? -ks[i] : ks[i]); k++)
			p *= FLT_RADIX;
		x = ks[i] >= 0 ? ms[i] * p : ms[i] / p;
		r = ilogbf(x);
		pc_check(r == ks[i], "ilogbf(%g) returned %g, expected %d", (double)x, (double)r, ks[i]);
	}
}

static void run_l(void)
{
	static const long double ms[] = { 1.0L, 1.5L, -1.0L, -1.5L, 1.0L, 1.0L, 1.5L, -1.0L };
	static const int ks[] = { 0, 1, 3, 3, -2, -7, 5, -3 };
	volatile long double x;
	volatile int r;
	long double p;
	int i, k;

	for (i = 0; i < 8; i++) {
		p = 1;
		for (k = 0; k < (ks[i] < 0 ? -ks[i] : ks[i]); k++)
			p *= FLT_RADIX;
		x = ks[i] >= 0 ? ms[i] * p : ms[i] / p;
		r = ilogbl(x);
		pc_check(r == ks[i], "ilogbl(%g) returned %g, expected %d", (double)x, (double)r, ks[i]);
	}
}

int main(void)
{
	pc_begin("XSH.ilogb.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
