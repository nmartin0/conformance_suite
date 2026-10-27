/*
 * lround() does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lround.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -100.5, -2.75, -2.5, -1.5, -0.5, -0.25, 0.25, 0.5, 1.5, 2.5, 2.75, 100.5 };
	static const long want[] = { -101L, -3L, -3L, -2L, -1L, 0L, 0L, 1L, 2L, 3L, 3L, 101L };
	volatile double x;
	volatile long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = lround(x);
			pc_check(r == want[i], "lround(%g) returned %lld, expected %lld (rounding mode %d)", (double)x, (long long)r, (long long)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -100.5f, -2.75f, -2.5f, -1.5f, -0.5f, -0.25f, 0.25f, 0.5f, 1.5f, 2.5f, 2.75f, 100.5f };
	static const long want[] = { -101L, -3L, -3L, -2L, -1L, 0L, 0L, 1L, 2L, 3L, 3L, 101L };
	volatile float x;
	volatile long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = lroundf(x);
			pc_check(r == want[i], "lroundf(%g) returned %lld, expected %lld (rounding mode %d)", (double)x, (long long)r, (long long)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -100.5L, -2.75L, -2.5L, -1.5L, -0.5L, -0.25L, 0.25L, 0.5L, 1.5L, 2.5L, 2.75L, 100.5L };
	static const long want[] = { -101L, -3L, -3L, -2L, -1L, 0L, 0L, 1L, 2L, 3L, 3L, 101L };
	volatile long double x;
	volatile long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = lroundl(x);
			pc_check(r == want[i], "lroundl(%g) returned %lld, expected %lld (rounding mode %d)", (double)x, (long long)r, (long long)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.lround.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
