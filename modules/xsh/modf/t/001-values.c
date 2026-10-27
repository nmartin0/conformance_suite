/*
 * modf() splits a number into signed fractional and integral parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 3.75, -3.75, 5.0, 0.25, -0.25, 100.5, -2.5, 0.0625 };
	static const double wf[] = { 0.75, -0.75, 0.0, 0.25, -0.25, 0.5, -0.5, 0.0625 };
	static const double wi[] = { 3.0, -3.0, 5.0, 0.0, (-0.0), 100.0, -2.0, 0.0 };
	volatile double x, r;
	double ip;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		ip = 99;
		r = modf(x, &ip);
		pc_check(r == wf[i] && ip == wi[i], "modf(%g, &ip) returned %g and stored %g, expected %g and %g", (double)x, (double)r, (double)ip, (double)wf[i], (double)wi[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 3.75f, -3.75f, 5.0f, 0.25f, -0.25f, 100.5f, -2.5f, 0.0625f };
	static const float wf[] = { 0.75f, -0.75f, 0.0f, 0.25f, -0.25f, 0.5f, -0.5f, 0.0625f };
	static const float wi[] = { 3.0f, -3.0f, 5.0f, 0.0f, (-0.0f), 100.0f, -2.0f, 0.0f };
	volatile float x, r;
	float ip;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		ip = 99;
		r = modff(x, &ip);
		pc_check(r == wf[i] && ip == wi[i], "modff(%g, &ip) returned %g and stored %g, expected %g and %g", (double)x, (double)r, (double)ip, (double)wf[i], (double)wi[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 3.75L, -3.75L, 5.0L, 0.25L, -0.25L, 100.5L, -2.5L, 0.0625L };
	static const long double wf[] = { 0.75L, -0.75L, 0.0L, 0.25L, -0.25L, 0.5L, -0.5L, 0.0625L };
	static const long double wi[] = { 3.0L, -3.0L, 5.0L, 0.0L, (-0.0L), 100.0L, -2.0L, 0.0L };
	volatile long double x, r;
	long double ip;
	int i;

	for (i = 0; i < 8; i++) {
		x = in[i];
		ip = 99;
		r = modfl(x, &ip);
		pc_check(r == wf[i] && ip == wi[i], "modfl(%g, &ip) returned %g and stored %g, expected %g and %g", (double)x, (double)r, (double)ip, (double)wf[i], (double)wi[i]);
	}
}

int main(void)
{
	pc_begin("XSH.modf.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
