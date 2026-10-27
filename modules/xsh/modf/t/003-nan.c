/*
 * modf() of a NaN returns a NaN and stores a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.003
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	double ip;

	x = (double)NAN;
	ip = 99;
	r = modf(x, &ip);
	pc_check(isnan(r), "modf(NaN, &ip) returned %g, not a NaN", (double)r);
	pc_check(isnan(ip), "modf(NaN, &ip) stored %g, not a NaN", (double)ip);
}

static void run_f(void)
{
	volatile float x, r;
	float ip;

	x = (float)NAN;
	ip = 99;
	r = modff(x, &ip);
	pc_check(isnan(r), "modff(NaN, &ip) returned %g, not a NaN", (double)r);
	pc_check(isnan(ip), "modff(NaN, &ip) stored %g, not a NaN", (double)ip);
}

static void run_l(void)
{
	volatile long double x, r;
	long double ip;

	x = (long double)NAN;
	ip = 99;
	r = modfl(x, &ip);
	pc_check(isnan(r), "modfl(NaN, &ip) returned %g, not a NaN", (double)r);
	pc_check(isnan(ip), "modfl(NaN, &ip) stored %g, not a NaN", (double)ip);
}

int main(void)
{
	pc_begin("XSH.modf.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
