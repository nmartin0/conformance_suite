/*
 * round() rounds halfway cases away from zero in every rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.round.008
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -2.5, -1.5, -0.5, 0.5, 1.5, 2.5, -2.75, 2.75 };
	static const double want[] = { -3.0, -2.0, -1.0, 1.0, 2.0, 3.0, -3.0, 3.0 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = round(x);
			pc_check(r == want[i], "round(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -2.5f, -1.5f, -0.5f, 0.5f, 1.5f, 2.5f, -2.75f, 2.75f };
	static const float want[] = { -3.0f, -2.0f, -1.0f, 1.0f, 2.0f, 3.0f, -3.0f, 3.0f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = roundf(x);
			pc_check(r == want[i], "roundf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -2.5L, -1.5L, -0.5L, 0.5L, 1.5L, 2.5L, -2.75L, 2.75L };
	static const long double want[] = { -3.0L, -2.0L, -1.0L, 1.0L, 2.0L, 3.0L, -3.0L, 3.0L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = roundl(x);
			pc_check(r == want[i], "roundl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.round.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
