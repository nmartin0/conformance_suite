/*
 * rint() and nearbyint() give the same value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rint.007
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -3.5, -2.75, -2.5, -1.5, -1.25, -0.75, 0.75, 1.25, 1.5, 2.5, 2.75, 3.5 };
	volatile double x, r, n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rint(x);
			n = nearbyint(x);
			pc_check(r == n, "rint(%g) returned %g but nearbyint returned %g (rounding mode %d)", (double)x, (double)r, (double)n, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -3.5f, -2.75f, -2.5f, -1.5f, -1.25f, -0.75f, 0.75f, 1.25f, 1.5f, 2.5f, 2.75f, 3.5f };
	volatile float x, r, n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rintf(x);
			n = nearbyintf(x);
			pc_check(r == n, "rintf(%g) returned %g but nearbyintf returned %g (rounding mode %d)", (double)x, (double)r, (double)n, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -3.5L, -2.75L, -2.5L, -1.5L, -1.25L, -0.75L, 0.75L, 1.25L, 1.5L, 2.5L, 2.75L, 3.5L };
	volatile long double x, r, n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rintl(x);
			n = nearbyintl(x);
			pc_check(r == n, "rintl(%g) returned %g but nearbyintl returned %g (rounding mode %d)", (double)x, (double)r, (double)n, mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.rint.007");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(1);
	pc_pass();
}
