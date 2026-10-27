/*
 * scalbln() computes x * FLT_RADIX^n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.010
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 1.5, 1.0, -3.0, 0.5, 3.0, 0.75, 2.5 };
	static const long ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	volatile double x, r;
	volatile long n;
	double p, want;
	int i, k;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		p = 1;
		for (k = 0; k < (ns[i] < 0 ? -ns[i] : ns[i]); k++)
			p *= FLT_RADIX;
		want = ns[i] >= 0 ? in[i] * p : in[i] / p;
		r = scalbln(x, n);
		pc_check(r == want, "scalbln(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want);
	}
}

static void run_f(void)
{
	static const float in[] = { 1.5f, 1.0f, -3.0f, 0.5f, 3.0f, 0.75f, 2.5f };
	static const long ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	volatile float x, r;
	volatile long n;
	float p, want;
	int i, k;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		p = 1;
		for (k = 0; k < (ns[i] < 0 ? -ns[i] : ns[i]); k++)
			p *= FLT_RADIX;
		want = ns[i] >= 0 ? in[i] * p : in[i] / p;
		r = scalblnf(x, n);
		pc_check(r == want, "scalblnf(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want);
	}
}

static void run_l(void)
{
	static const long double in[] = { 1.5L, 1.0L, -3.0L, 0.5L, 3.0L, 0.75L, 2.5L };
	static const long ns[] = { 3, -2, 2, 1, 0, -4, 5 };
	volatile long double x, r;
	volatile long n;
	long double p, want;
	int i, k;

	for (i = 0; i < 7; i++) {
		x = in[i];
		n = ns[i];
		p = 1;
		for (k = 0; k < (ns[i] < 0 ? -ns[i] : ns[i]); k++)
			p *= FLT_RADIX;
		want = ns[i] >= 0 ? in[i] * p : in[i] / p;
		r = scalblnl(x, n);
		pc_check(r == want, "scalblnl(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want);
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.010");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
