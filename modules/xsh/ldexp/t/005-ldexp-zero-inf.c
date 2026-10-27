/*
 * ldexp() of a zero or an infinity returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldexp.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0), (double)INFINITY, (-(double)INFINITY) };
	static const int ns[] = { 5, 5, -3, -3 };
	static const double want[] = { 0.0, (-0.0), (double)INFINITY, (-(double)INFINITY) };
	volatile double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = ldexp(x, n);
		pc_check(MT_SAME(r, want[i]), "ldexp(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f), (float)INFINITY, (-(float)INFINITY) };
	static const int ns[] = { 5, 5, -3, -3 };
	static const float want[] = { 0.0f, (-0.0f), (float)INFINITY, (-(float)INFINITY) };
	volatile float x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = ldexpf(x, n);
		pc_check(MT_SAME(r, want[i]), "ldexpf(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L), (long double)INFINITY, (-(long double)INFINITY) };
	static const int ns[] = { 5, 5, -3, -3 };
	static const long double want[] = { 0.0L, (-0.0L), (long double)INFINITY, (-(long double)INFINITY) };
	volatile long double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = ldexpl(x, n);
		pc_check(MT_SAME(r, want[i]), "ldexpl(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.ldexp.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
