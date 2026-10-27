/*
 * scalbn() with exponent 0 returns x.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.006
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 3.0, -3.0, 0.1, -0.1 };
	static const int ns[] = { 0, 0, 0, 0 };
	static const double want[] = { 3.0, -3.0, 0.1, -0.1 };
	volatile double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = scalbn(x, n);
		pc_check(MT_SAME(r, want[i]), "scalbn(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 3.0f, -3.0f, 0.1f, -0.1f };
	static const int ns[] = { 0, 0, 0, 0 };
	static const float want[] = { 3.0f, -3.0f, 0.1f, -0.1f };
	volatile float x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = scalbnf(x, n);
		pc_check(MT_SAME(r, want[i]), "scalbnf(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 3.0L, -3.0L, 0.1L, -0.1L };
	static const int ns[] = { 0, 0, 0, 0 };
	static const long double want[] = { 3.0L, -3.0L, 0.1L, -0.1L };
	volatile long double x, r;
	volatile int n;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		n = ns[i];
		r = scalbnl(x, n);
		pc_check(MT_SAME(r, want[i]), "scalbnl(%g, %lld) returned %g, expected %g", (double)x, (long long)ns[i], (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
