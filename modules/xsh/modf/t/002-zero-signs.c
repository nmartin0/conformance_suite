/*
 * The parts of modf() carry the sign of the argument, zeros included.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 3.0, -3.0, 0.25, -0.25 };
	static const double wf[] = { 0.0, -0.0, 0.25, -0.25 };
	static const double wi[] = { 3.0, -3.0, 0.0, -0.0 };
	volatile double x, r;
	double ip;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		ip = 99;
		r = modf(x, &ip);
		pc_check(MT_SAME(r, wf[i]) && MT_SAME(ip, wi[i]), "modf(%g, &ip) returned %g and stored %g, with a sign other than the argument's", (double)x, (double)r, (double)ip);
	}
}

static void run_f(void)
{
	static const float in[] = { 3.0f, -3.0f, 0.25f, -0.25f };
	static const float wf[] = { 0.0f, -0.0f, 0.25f, -0.25f };
	static const float wi[] = { 3.0f, -3.0f, 0.0f, -0.0f };
	volatile float x, r;
	float ip;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		ip = 99;
		r = modff(x, &ip);
		pc_check(MT_SAME(r, wf[i]) && MT_SAME(ip, wi[i]), "modff(%g, &ip) returned %g and stored %g, with a sign other than the argument's", (double)x, (double)r, (double)ip);
	}
}

static void run_l(void)
{
	static const long double in[] = { 3.0L, -3.0L, 0.25L, -0.25L };
	static const long double wf[] = { 0.0L, -0.0L, 0.25L, -0.25L };
	static const long double wi[] = { 3.0L, -3.0L, 0.0L, -0.0L };
	volatile long double x, r;
	long double ip;
	int i;

	for (i = 0; i < 4; i++) {
		x = in[i];
		ip = 99;
		r = modfl(x, &ip);
		pc_check(MT_SAME(r, wf[i]) && MT_SAME(ip, wi[i]), "modfl(%g, &ip) returned %g and stored %g, with a sign other than the argument's", (double)x, (double)r, (double)ip);
	}
}

int main(void)
{
	pc_begin("XSH.modf.002");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
