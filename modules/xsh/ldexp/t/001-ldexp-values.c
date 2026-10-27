/*
 * ldexp() computes x * 2^exp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldexp.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 1.5, 1.0, -3.0, 0.5, 3.0, 0.75, 2.5 };
	static const int ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	static const double want[] = { 12.0, 0.25, -12.0, 1.0, 3.0, 0.046875, 80.0 };
	volatile double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		r = ldexp(x, n);
		pc_check(r == want[i], "ldexp(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 1.5f, 1.0f, -3.0f, 0.5f, 3.0f, 0.75f, 2.5f };
	static const int ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	static const float want[] = { 12.0f, 0.25f, -12.0f, 1.0f, 3.0f, 0.046875f, 80.0f };
	volatile float x, r;
	volatile int n;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		r = ldexpf(x, n);
		pc_check(r == want[i], "ldexpf(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 1.5L, 1.0L, -3.0L, 0.5L, 3.0L, 0.75L, 2.5L };
	static const int ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	static const long double want[] = { 12.0L, 0.25L, -12.0L, 1.0L, 3.0L, 0.046875L, 80.0L };
	volatile long double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		r = ldexpl(x, n);
		pc_check(r == want[i], "ldexpl(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.ldexp.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
