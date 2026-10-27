/*
 * The result of fabs() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fabs.005
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -0.1, 0.1 };
	static const double want[] = { 0.1, 0.1 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = fabs(x);
			pc_check(r == want[i], "fabs(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -0.1f, 0.1f };
	static const float want[] = { 0.1f, 0.1f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = fabsf(x);
			pc_check(r == want[i], "fabsf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -0.1L, 0.1L };
	static const long double want[] = { 0.1L, 0.1L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = fabsl(x);
			pc_check(r == want[i], "fabsl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.fabs.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
