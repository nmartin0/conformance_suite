/*
 * The result of remainder() does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remainder.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 5.0, 7.0, 5.0, -5.0, 1.0, 3.0, 6.0, 1.5, -1.5, 2.5, -7.0, 5.5 };
	static const double in2[] = { 3.0, 2.0, 2.0, 2.0, 4.0, 4.0, 3.0, 1.0, 1.0, 1.0, 2.0, 2.0 };
	static const double want[] = { -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 0.0, -0.5, 0.5, 0.5, 1.0, -0.5 };
	volatile double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in1[i];
			y = in2[i];
			r = remainder(x, y);
			pc_check(r == want[i], "remainder(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in1[] = { 5.0f, 7.0f, 5.0f, -5.0f, 1.0f, 3.0f, 6.0f, 1.5f, -1.5f, 2.5f, -7.0f, 5.5f };
	static const float in2[] = { 3.0f, 2.0f, 2.0f, 2.0f, 4.0f, 4.0f, 3.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f };
	static const float want[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, -0.5f };
	volatile float x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in1[i];
			y = in2[i];
			r = remainderf(x, y);
			pc_check(r == want[i], "remainderf(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in1[] = { 5.0L, 7.0L, 5.0L, -5.0L, 1.0L, 3.0L, 6.0L, 1.5L, -1.5L, 2.5L, -7.0L, 5.5L };
	static const long double in2[] = { 3.0L, 2.0L, 2.0L, 2.0L, 4.0L, 4.0L, 3.0L, 1.0L, 1.0L, 1.0L, 2.0L, 2.0L };
	static const long double want[] = { -1.0L, -1.0L, 1.0L, -1.0L, 1.0L, -1.0L, 0.0L, -0.5L, 0.5L, 0.5L, 1.0L, -0.5L };
	volatile long double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in1[i];
			y = in2[i];
			r = remainderl(x, y);
			pc_check(r == want[i], "remainderl(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.remainder.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
