/*
 * The result of copysign() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 1.1, -0.1 };
	static const double in2[] = { -1.0, 2.0 };
	static const double want[] = { -1.1, 0.1 };
	volatile double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = copysign(x, y);
			pc_check(r == want[i], "copysign(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in1[] = { 1.1f, -0.1f };
	static const float in2[] = { -1.0f, 2.0f };
	static const float want[] = { -1.1f, 0.1f };
	volatile float x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = copysignf(x, y);
			pc_check(r == want[i], "copysignf(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in1[] = { 1.1L, -0.1L };
	static const long double in2[] = { -1.0L, 2.0L };
	static const long double want[] = { -1.1L, 0.1L };
	volatile long double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = copysignl(x, y);
			pc_check(r == want[i], "copysignl(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.copysign.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
