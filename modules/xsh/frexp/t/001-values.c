/*
 * frexp() splits a number into a fraction in [1/2, 1) and a power of two.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 8.0, 1.0, -6.0, 0.1875, 1024.0, 0.5, 3.0, -0.25 };
	static const double wf[] = { 0.5, 0.5, -0.75, 0.75, 0.5, 0.5, 0.75, -0.5 };
	static const int we[] = { 4, 1, 3, -2, 11, 0, 2, -1 };
	volatile double x, r;
	int i, e;

	for (i = 0; i < 8; i++) {
		x = in[i];
		e = 12345;
		r = frexp(x, &e);
		pc_check(r == wf[i] && e == we[i], "frexp(%g, &e) returned %g with exponent %d, expected %g and %d", (double)x, (double)r, e, (double)wf[i], we[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 8.0f, 1.0f, -6.0f, 0.1875f, 1024.0f, 0.5f, 3.0f, -0.25f };
	static const float wf[] = { 0.5f, 0.5f, -0.75f, 0.75f, 0.5f, 0.5f, 0.75f, -0.5f };
	static const int we[] = { 4, 1, 3, -2, 11, 0, 2, -1 };
	volatile float x, r;
	int i, e;

	for (i = 0; i < 8; i++) {
		x = in[i];
		e = 12345;
		r = frexpf(x, &e);
		pc_check(r == wf[i] && e == we[i], "frexpf(%g, &e) returned %g with exponent %d, expected %g and %d", (double)x, (double)r, e, (double)wf[i], we[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 8.0L, 1.0L, -6.0L, 0.1875L, 1024.0L, 0.5L, 3.0L, -0.25L };
	static const long double wf[] = { 0.5L, 0.5L, -0.75L, 0.75L, 0.5L, 0.5L, 0.75L, -0.5L };
	static const int we[] = { 4, 1, 3, -2, 11, 0, 2, -1 };
	volatile long double x, r;
	int i, e;

	for (i = 0; i < 8; i++) {
		x = in[i];
		e = 12345;
		r = frexpl(x, &e);
		pc_check(r == wf[i] && e == we[i], "frexpl(%g, &e) returned %g with exponent %d, expected %g and %d", (double)x, (double)r, e, (double)wf[i], we[i]);
	}
}

int main(void)
{
	pc_begin("XSH.frexp.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
