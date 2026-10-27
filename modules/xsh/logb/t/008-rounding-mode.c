/*
 * The result of logb() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.008
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 10.0, 0.5 };
	static const double want[] = { 3.0, -1.0 };
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = logb(x);
			pc_check(r == want[i], "logb(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { 10.0f, 0.5f };
	static const float want[] = { 3.0f, -1.0f };
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = logbf(x);
			pc_check(r == want[i], "logbf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { 10.0L, 0.5L };
	static const long double want[] = { 3.0L, -1.0L };
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			r = logbl(x);
			pc_check(r == want[i], "logbl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.logb.008");
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
