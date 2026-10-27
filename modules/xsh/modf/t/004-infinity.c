/*
 * modf() of an infinity returns a zero and stores the infinity.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { (double)INFINITY, -(double)INFINITY };
	static const double wf[] = { 0.0, -0.0 };
	volatile double x, r;
	double ip;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		ip = 99;
		r = modf(x, &ip);
		pc_check(MT_SAME(r, wf[i]), "modf(%g, &ip) returned %g, expected a zero with the sign of the argument", (double)x, (double)r);
		pc_check(ip == in[i], "modf(%g, &ip) stored %g, expected the argument", (double)x, (double)ip);
	}
}

static void run_f(void)
{
	static const float in[] = { (float)INFINITY, -(float)INFINITY };
	static const float wf[] = { 0.0f, -0.0f };
	volatile float x, r;
	float ip;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		ip = 99;
		r = modff(x, &ip);
		pc_check(MT_SAME(r, wf[i]), "modff(%g, &ip) returned %g, expected a zero with the sign of the argument", (double)x, (double)r);
		pc_check(ip == in[i], "modff(%g, &ip) stored %g, expected the argument", (double)x, (double)ip);
	}
}

static void run_l(void)
{
	static const long double in[] = { (long double)INFINITY, -(long double)INFINITY };
	static const long double wf[] = { 0.0L, -0.0L };
	volatile long double x, r;
	long double ip;
	int i;

	for (i = 0; i < 2; i++) {
		x = in[i];
		ip = 99;
		r = modfl(x, &ip);
		pc_check(MT_SAME(r, wf[i]), "modfl(%g, &ip) returned %g, expected a zero with the sign of the argument", (double)x, (double)r);
		pc_check(ip == in[i], "modfl(%g, &ip) stored %g, expected the argument", (double)x, (double)ip);
	}
}

int main(void)
{
	pc_begin("XSH.modf.004");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
