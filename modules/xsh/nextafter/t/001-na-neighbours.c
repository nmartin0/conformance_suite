/*
 * nextafter() returns the adjacent representable value toward y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double xs[] = { 1.0, 1.0, -1.0, -1.0, 1.0, 1.0 };
	static const double ys[] = { 2.0, 0.0, -2.0, 0.0, 100.0, -100.0 };
	volatile double x, y, r;
	double want[6];
	int i;

	want[0] = 1 + DBL_EPSILON;
	want[1] = 1 - DBL_EPSILON / FLT_RADIX;
	want[2] = -(1 + DBL_EPSILON);
	want[3] = -(1 - DBL_EPSILON / FLT_RADIX);
	want[4] = 1 + DBL_EPSILON;
	want[5] = 1 - DBL_EPSILON / FLT_RADIX;
	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = nextafter(x, y);
		pc_check(r == want[i], "nextafter(%g, %g) returned %.20g, expected %.20g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float xs[] = { 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f };
	static const float ys[] = { 2.0f, 0.0f, -2.0f, 0.0f, 100.0f, -100.0f };
	volatile float x, y, r;
	float want[6];
	int i;

	want[0] = 1 + FLT_EPSILON;
	want[1] = 1 - FLT_EPSILON / FLT_RADIX;
	want[2] = -(1 + FLT_EPSILON);
	want[3] = -(1 - FLT_EPSILON / FLT_RADIX);
	want[4] = 1 + FLT_EPSILON;
	want[5] = 1 - FLT_EPSILON / FLT_RADIX;
	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = nextafterf(x, y);
		pc_check(r == want[i], "nextafterf(%g, %g) returned %.20g, expected %.20g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double xs[] = { 1.0L, 1.0L, -1.0L, -1.0L, 1.0L, 1.0L };
	static const long double ys[] = { 2.0L, 0.0L, -2.0L, 0.0L, 100.0L, -100.0L };
	volatile long double x, y, r;
	long double want[6];
	int i;

	want[0] = 1 + LDBL_EPSILON;
	want[1] = 1 - LDBL_EPSILON / FLT_RADIX;
	want[2] = -(1 + LDBL_EPSILON);
	want[3] = -(1 - LDBL_EPSILON / FLT_RADIX);
	want[4] = 1 + LDBL_EPSILON;
	want[5] = 1 - LDBL_EPSILON / FLT_RADIX;
	for (i = 0; i < 6; i++) {
		x = xs[i];
		y = ys[i];
		r = nextafterl(x, y);
		pc_check(r == want[i], "nextafterl(%g, %g) returned %.20g, expected %.20g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
