/*
 * frexp() of a zero returns that zero with its sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, -0.0 };
	volatile double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexp(x, &e);
		pc_check(MT_SAME(r, in[i]), "frexp(%g, &e) returned %g, expected the same zero", (double)x, (double)r);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, -0.0f };
	volatile float x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexpf(x, &e);
		pc_check(MT_SAME(r, in[i]), "frexpf(%g, &e) returned %g, expected the same zero", (double)x, (double)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, -0.0L };
	volatile long double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexpl(x, &e);
		pc_check(MT_SAME(r, in[i]), "frexpl(%g, &e) returned %g, expected the same zero", (double)x, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.frexp.004");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
