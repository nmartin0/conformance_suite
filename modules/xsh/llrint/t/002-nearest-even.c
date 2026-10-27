/*
 * llrint() rounds halfway cases to even to nearest.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llrint.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -3.5, -2.75, -2.5, -1.5, -1.25, -0.75, 0.75, 1.25, 1.5, 2.5, 2.75, 3.5, 0.5, 4.5, -4.5, -0.5 };
	static const long long want[] = { -4LL, -3LL, -2LL, -2LL, -1LL, -1LL, 1LL, 1LL, 2LL, 2LL, 3LL, 4LL, 0LL, 4LL, -4LL, 0LL };
	volatile double x;
	volatile long long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] != MT_NEAR || !mt_enter(m))
			continue;
		for (i = 0; i < 16; i++) {
			x = in[i];
			r = llrint(x);
			pc_check(r == want[i], "llrint(%g) returned %lld, expected %lld (to nearest, ties to even)", (double)x, (long long)r, (long long)want[i]);
		}
	}
	mt_leave();
}

static void run_f(void)
{
	static const float in[] = { -3.5f, -2.75f, -2.5f, -1.5f, -1.25f, -0.75f, 0.75f, 1.25f, 1.5f, 2.5f, 2.75f, 3.5f, 0.5f, 4.5f, -4.5f, -0.5f };
	static const long long want[] = { -4LL, -3LL, -2LL, -2LL, -1LL, -1LL, 1LL, 1LL, 2LL, 2LL, 3LL, 4LL, 0LL, 4LL, -4LL, 0LL };
	volatile float x;
	volatile long long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] != MT_NEAR || !mt_enter(m))
			continue;
		for (i = 0; i < 16; i++) {
			x = in[i];
			r = llrintf(x);
			pc_check(r == want[i], "llrintf(%g) returned %lld, expected %lld (to nearest, ties to even)", (double)x, (long long)r, (long long)want[i]);
		}
	}
	mt_leave();
}

static void run_l(void)
{
	static const long double in[] = { -3.5L, -2.75L, -2.5L, -1.5L, -1.25L, -0.75L, 0.75L, 1.25L, 1.5L, 2.5L, 2.75L, 3.5L, 0.5L, 4.5L, -4.5L, -0.5L };
	static const long long want[] = { -4LL, -3LL, -2LL, -2LL, -1LL, -1LL, 1LL, 1LL, 2LL, 2LL, 3LL, 4LL, 0LL, 4LL, -4LL, 0LL };
	volatile long double x;
	volatile long long r;
	int i, m;

	for (m = 0; m < MT_NMODES; m++) {
		if (mt_modes[m] != MT_NEAR || !mt_enter(m))
			continue;
		for (i = 0; i < 16; i++) {
			x = in[i];
			r = llrintl(x);
			pc_check(r == want[i], "llrintl(%g) returned %lld, expected %lld (to nearest, ties to even)", (double)x, (long long)r, (long long)want[i]);
		}
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.llrint.002");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(1);
	pc_pass();
}
