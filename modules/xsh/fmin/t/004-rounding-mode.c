/*
 * The result of fmin() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmin.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 1.1, -1.1 };
	static const double in2[] = { 2.2, -2.2 };
	static const double want[] = { 1.1, -2.2 };
	volatile double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = fmin(x, y);
			pc_check(r == want[i], "fmin(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in1[] = { 1.1f, -1.1f };
	static const float in2[] = { 2.2f, -2.2f };
	static const float want[] = { 1.1f, -2.2f };
	volatile float x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = fminf(x, y);
			pc_check(r == want[i], "fminf(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in1[] = { 1.1L, -1.1L };
	static const long double in2[] = { 2.2L, -2.2L };
	static const long double want[] = { 1.1L, -2.2L };
	volatile long double x, y, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in1[i];
			y = in2[i];
			r = fminl(x, y);
			pc_check(r == want[i], "fminl(%g, %g) returned %g, expected %g (rounding mode %d)", (double)x, (double)y, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.fmin.004");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
