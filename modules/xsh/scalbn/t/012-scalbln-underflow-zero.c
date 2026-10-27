/*
 * scalbln() of an unrepresentably small result is zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.012
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 1.0, -1.0 };
	static const long ns[] = { LONG_MIN, LONG_MIN };
	static const double want[] = { 0.0, (-0.0) };
	volatile double x, r;
	volatile long n;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		n = ns[i];
		r = scalbln(x, n);
		pc_check(r == want[i], "scalbln(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 1.0f, -1.0f };
	static const long ns[] = { LONG_MIN, LONG_MIN };
	static const float want[] = { 0.0f, (-0.0f) };
	volatile float x, r;
	volatile long n;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		n = ns[i];
		r = scalblnf(x, n);
		pc_check(r == want[i], "scalblnf(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 1.0L, -1.0L };
	static const long ns[] = { LONG_MIN, LONG_MIN };
	static const long double want[] = { 0.0L, (-0.0L) };
	volatile long double x, r;
	volatile long n;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		n = ns[i];
		r = scalblnl(x, n);
		pc_check(r == want[i], "scalblnl(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.012");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
