/*
 * frexp() of an infinity returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.006
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, -(double)INFINITY };
	volatile double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexp(x, &e);
		pc_check(r == in[i], "frexp(%g, &e) returned %g, expected the argument", (double)x, (double)r);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)INFINITY, -(float)INFINITY };
	volatile float x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexpf(x, &e);
		pc_check(r == in[i], "frexpf(%g, &e) returned %g, expected the argument", (double)x, (double)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)INFINITY, -(long double)INFINITY };
	volatile long double x, r;
	int i, e;

	for (i = 0; i < 2; i++) {
		x = in[i];
		r = frexpl(x, &e);
		pc_check(r == in[i], "frexpl(%g, &e) returned %g, expected the argument", (double)x, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.frexp.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
