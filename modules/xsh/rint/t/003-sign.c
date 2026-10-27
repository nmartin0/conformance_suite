/*
 * The result of rint() has the sign of x.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rint.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -0.75, -0.25, 0.25, 0.75 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 4; i++) {
			x = in[i];
			r = rint(x);
			pc_check((signbit(r) != 0) == (in[i] < 0), "rint(%g) returned %g, which has the wrong sign (rounding mode %d)", (double)x, (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -0.75f, -0.25f, 0.25f, 0.75f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 4; i++) {
			x = in[i];
			r = rintf(x);
			pc_check((signbit(r) != 0) == (in[i] < 0), "rintf(%g) returned %g, which has the wrong sign (rounding mode %d)", (double)x, (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -0.75L, -0.25L, 0.25L, 0.75L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 4; i++) {
			x = in[i];
			r = rintl(x);
			pc_check((signbit(r) != 0) == (in[i] < 0), "rintl(%g) returned %g, which has the wrong sign (rounding mode %d)", (double)x, (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.rint.003");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(1);
	pc_pass();
}
