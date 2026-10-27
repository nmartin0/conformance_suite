/*
 * The result of trunc() does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.trunc.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -2.5, -1.5, 1.5, 2.5, -2.75, 2.75, 0.75, -0.75 };
	static const double want[] = { -2.0, -1.0, 1.0, 2.0, -2.0, 2.0, 0.0, 0.0 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = trunc(x);
			pc_check(r == want[i], "trunc(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -2.5f, -1.5f, 1.5f, 2.5f, -2.75f, 2.75f, 0.75f, -0.75f };
	static const float want[] = { -2.0f, -1.0f, 1.0f, 2.0f, -2.0f, 2.0f, 0.0f, 0.0f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = truncf(x);
			pc_check(r == want[i], "truncf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -2.5L, -1.5L, 1.5L, 2.5L, -2.75L, 2.75L, 0.75L, -0.75L };
	static const long double want[] = { -2.0L, -1.0L, 1.0L, 2.0L, -2.0L, 2.0L, 0.0L, 0.0L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = truncl(x);
			pc_check(r == want[i], "truncl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.trunc.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
