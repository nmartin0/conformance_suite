/*
 * The result of scalbln() is exact and does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.017
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 1.1, -0.1 };
	static const long ns[] = { 3, -2 };
	volatile double x, r;
	volatile long n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			n = ns[i];
			r = scalbln(x, n);
			pc_check(r == in[i] * (ns[i] > 0 ? 8 : 0.25), "scalbln(%g, %lld) returned %g (rounding mode %d)", (double)x, (long long)ns[i], (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { 1.1f, -0.1f };
	static const long ns[] = { 3, -2 };
	volatile float x, r;
	volatile long n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			n = ns[i];
			r = scalblnf(x, n);
			pc_check(r == in[i] * (ns[i] > 0 ? 8 : 0.25f), "scalblnf(%g, %lld) returned %g (rounding mode %d)", (double)x, (long long)ns[i], (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { 1.1L, -0.1L };
	static const long ns[] = { 3, -2 };
	volatile long double x, r;
	volatile long n;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 2; i++) {
			x = in[i];
			n = ns[i];
			r = scalblnl(x, n);
			pc_check(r == in[i] * (ns[i] > 0 ? 8 : 0.25L), "scalblnl(%g, %lld) returned %g (rounding mode %d)", (double)x, (long long)ns[i], (double)r, mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.scalbn.017");
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
