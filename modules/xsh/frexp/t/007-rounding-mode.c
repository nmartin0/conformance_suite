/*
 * The result of frexp() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 10.0, -10.0 };
	static const double wf[] = { 0.625, -0.625 };
	volatile double x, r;
	int i, m, e;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			e = 0;
			r = frexp(x, &e);
			pc_check(r == wf[i] && e == 4, "frexp(%g, &e) returned %g with exponent %d (rounding mode %d)", (double)x, (double)r, e, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { 10.0f, -10.0f };
	static const float wf[] = { 0.625f, -0.625f };
	volatile float x, r;
	int i, m, e;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			e = 0;
			r = frexpf(x, &e);
			pc_check(r == wf[i] && e == 4, "frexpf(%g, &e) returned %g with exponent %d (rounding mode %d)", (double)x, (double)r, e, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { 10.0L, -10.0L };
	static const long double wf[] = { 0.625L, -0.625L };
	volatile long double x, r;
	int i, m, e;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			e = 0;
			r = frexpl(x, &e);
			pc_check(r == wf[i] && e == 4, "frexpl(%g, &e) returned %g with exponent %d (rounding mode %d)", (double)x, (double)r, e, mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.frexp.007");
	MT_NEED_MX();
	if (FLT_RADIX != 2)
		pc_notinuse("the radix is not 2");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
