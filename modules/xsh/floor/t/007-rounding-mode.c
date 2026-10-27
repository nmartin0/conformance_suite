/*
 * The result of floor() does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.floor.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -2.5, -1.5, 1.5, 2.5, -2.75, 2.75, 0.75, -0.75 };
	static const double want[] = { -3.0, -2.0, 1.0, 2.0, -3.0, 2.0, 0.0, -1.0 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = floor(x);
			pc_check(r == want[i], "floor(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -2.5f, -1.5f, 1.5f, 2.5f, -2.75f, 2.75f, 0.75f, -0.75f };
	static const float want[] = { -3.0f, -2.0f, 1.0f, 2.0f, -3.0f, 2.0f, 0.0f, -1.0f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = floorf(x);
			pc_check(r == want[i], "floorf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -2.5L, -1.5L, 1.5L, 2.5L, -2.75L, 2.75L, 0.75L, -0.75L };
	static const long double want[] = { -3.0L, -2.0L, 1.0L, 2.0L, -3.0L, 2.0L, 0.0L, -1.0L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 8; i++) {
			x = in[i];
			r = floorl(x);
			pc_check(r == want[i], "floorl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.floor.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
