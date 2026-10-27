/*
 * rint() follows a directed rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rint.001
 */
#include "mt.h"

static int ndir;

static void run_d(void)
{
	static const double in[] = { -3.5, -2.75, -2.5, -1.5, -1.25, -0.75, 0.75, 1.25, 1.5, 2.5, 2.75, 3.5 };
	static const double down[] = { -4.0, -3.0, -3.0, -2.0, -2.0, -1.0, 0.0, 1.0, 1.0, 2.0, 2.0, 3.0 };
	static const double up[] = { -3.0, -2.0, -2.0, -1.0, -1.0, 0.0, 1.0, 2.0, 2.0, 3.0, 3.0, 4.0 };
	static const double zero[] = { -3.0, -2.0, -2.0, -1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 3.0 };
	const double *want;
	volatile double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] == MT_DOWN)
			want = down;
		else if (mt_modes[m] == MT_UP)
			want = up;
		else if (mt_modes[m] == MT_ZERO)
			want = zero;
		else
			continue;
		if (!mt_enter(m))
			continue;
		ndir++;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rint(x);
			pc_check(r == want[i], "rint(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -3.5f, -2.75f, -2.5f, -1.5f, -1.25f, -0.75f, 0.75f, 1.25f, 1.5f, 2.5f, 2.75f, 3.5f };
	static const float down[] = { -4.0f, -3.0f, -3.0f, -2.0f, -2.0f, -1.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f };
	static const float up[] = { -3.0f, -2.0f, -2.0f, -1.0f, -1.0f, 0.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f, 4.0f };
	static const float zero[] = { -3.0f, -2.0f, -2.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f };
	const float *want;
	volatile float x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] == MT_DOWN)
			want = down;
		else if (mt_modes[m] == MT_UP)
			want = up;
		else if (mt_modes[m] == MT_ZERO)
			want = zero;
		else
			continue;
		if (!mt_enter(m))
			continue;
		ndir++;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rintf(x);
			pc_check(r == want[i], "rintf(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -3.5L, -2.75L, -2.5L, -1.5L, -1.25L, -0.75L, 0.75L, 1.25L, 1.5L, 2.5L, 2.75L, 3.5L };
	static const long double down[] = { -4.0L, -3.0L, -3.0L, -2.0L, -2.0L, -1.0L, 0.0L, 1.0L, 1.0L, 2.0L, 2.0L, 3.0L };
	static const long double up[] = { -3.0L, -2.0L, -2.0L, -1.0L, -1.0L, 0.0L, 1.0L, 2.0L, 2.0L, 3.0L, 3.0L, 4.0L };
	static const long double zero[] = { -3.0L, -2.0L, -2.0L, -1.0L, -1.0L, 0.0L, 0.0L, 1.0L, 1.0L, 2.0L, 2.0L, 3.0L };
	const long double *want;
	volatile long double x, r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] == MT_DOWN)
			want = down;
		else if (mt_modes[m] == MT_UP)
			want = up;
		else if (mt_modes[m] == MT_ZERO)
			want = zero;
		else
			continue;
		if (!mt_enter(m))
			continue;
		ndir++;
		for (i = 0; i < 12; i++) {
			x = in[i];
			r = rintl(x);
			pc_check(r == want[i], "rintl(%g) returned %g, expected %g (rounding mode %d)", (double)x, (double)r, (double)want[i], mt_modes[m]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.rint.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	if (ndir == 0) {
		mt_leave();
		pc_notinuse("no directed rounding direction can be selected");
	}
	pc_pass();
}
