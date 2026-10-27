/*
 * ilogb() of an infinity returns INT_MAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, -(double)INFINITY };
	volatile double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogb(x);
		pc_check(r == INT_MAX, "ilogb(%g) returned %d, expected INT_MAX", (double)x, (int)r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)INFINITY, -(float)INFINITY };
	volatile float x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogbf(x);
		pc_check(r == INT_MAX, "ilogbf(%g) returned %d, expected INT_MAX", (double)x, (int)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)INFINITY, -(long double)INFINITY };
	volatile long double x;
	volatile int r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = ilogbl(x);
		pc_check(r == INT_MAX, "ilogbl(%g) returned %d, expected INT_MAX", (double)x, (int)r);
	}
}

int main(void)
{
	pc_begin("XSH.ilogb.005");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
