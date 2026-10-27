/*
 * frexp() of a zero stores exponent 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, -0.0 };
	volatile double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		e = 7;
		r = frexp(x, &e);
		pc_check(r == 0, "frexp(%g, &e) returned %g, expected a zero", (double)x, (double)r);
		pc_check(!MT_MX || e == 0, "frexp(%g, &e) stored exponent %d, expected 0", (double)x, e);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, -0.0f };
	volatile float x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		e = 7;
		r = frexpf(x, &e);
		pc_check(r == 0, "frexpf(%g, &e) returned %g, expected a zero", (double)x, (double)r);
		pc_check(!MT_MX || e == 0, "frexpf(%g, &e) stored exponent %d, expected 0", (double)x, e);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, -0.0L };
	volatile long double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		e = 7;
		r = frexpl(x, &e);
		pc_check(r == 0, "frexpl(%g, &e) returned %g, expected a zero", (double)x, (double)r);
		pc_check(!MT_MX || e == 0, "frexpl(%g, &e) stored exponent %d, expected 0", (double)x, e);
	}
}

int main(void)
{
	pc_begin("XSH.frexp.003");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
