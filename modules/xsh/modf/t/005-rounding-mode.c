/*
 * The result of modf() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 2.75, -2.75 };
	static const double wf[] = { 0.75, -0.75 };
	static const double wi[] = { 2.0, -2.0 };
	volatile double x, r;
	double ip;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = modf(x, &ip);
			pc_check(r == wf[i] && ip == wi[i], "modf(%g, &ip) returned %g and stored %g (rounding mode %d)", (double)x, (double)r, (double)ip, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { 2.75f, -2.75f };
	static const float wf[] = { 0.75f, -0.75f };
	static const float wi[] = { 2.0f, -2.0f };
	volatile float x, r;
	float ip;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = modff(x, &ip);
			pc_check(r == wf[i] && ip == wi[i], "modff(%g, &ip) returned %g and stored %g (rounding mode %d)", (double)x, (double)r, (double)ip, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { 2.75L, -2.75L };
	static const long double wf[] = { 0.75L, -0.75L };
	static const long double wi[] = { 2.0L, -2.0L };
	volatile long double x, r;
	long double ip;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = modfl(x, &ip);
			pc_check(r == wf[i] && ip == wi[i], "modfl(%g, &ip) returned %g and stored %g (rounding mode %d)", (double)x, (double)r, (double)ip, mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.modf.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
