/*
 * nextafter() returns y when x equals y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 3.0, -2.5, 0.1 };
	static const double in2[] = { 3.0, -2.5, 0.1 };
	static const double want[] = { 3.0, -2.5, 0.1 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafter(x, y);
		pc_check(r == want[i], "nextafter(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 3.0f, -2.5f, 0.1f };
	static const float in2[] = { 3.0f, -2.5f, 0.1f };
	static const float want[] = { 3.0f, -2.5f, 0.1f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterf(x, y);
		pc_check(r == want[i], "nextafterf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 3.0L, -2.5L, 0.1L };
	static const long double in2[] = { 3.0L, -2.5L, 0.1L };
	static const long double want[] = { 3.0L, -2.5L, 0.1L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 3; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterl(x, y);
		pc_check(r == want[i], "nextafterl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
