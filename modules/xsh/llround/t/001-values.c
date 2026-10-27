/*
 * llround() rounds halfway cases away from zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llround.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -100.5, -2.75, -2.5, -1.5, -0.5, -0.25, 0.25, 0.5, 1.5, 2.5, 2.75, 100.5 };
	static const long long want[] = { -101LL, -3LL, -3LL, -2LL, -1LL, 0LL, 0LL, 1LL, 2LL, 3LL, 3LL, 101LL };
	volatile double x;
	volatile long long r;
	int i, m;

	(void)m;
	for (i = 0; i < 12; i++) {
		x = in[i];
		r = llround(x);
		pc_check(r == want[i], "llround(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { -100.5f, -2.75f, -2.5f, -1.5f, -0.5f, -0.25f, 0.25f, 0.5f, 1.5f, 2.5f, 2.75f, 100.5f };
	static const long long want[] = { -101LL, -3LL, -3LL, -2LL, -1LL, 0LL, 0LL, 1LL, 2LL, 3LL, 3LL, 101LL };
	volatile float x;
	volatile long long r;
	int i, m;

	(void)m;
	for (i = 0; i < 12; i++) {
		x = in[i];
		r = llroundf(x);
		pc_check(r == want[i], "llroundf(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { -100.5L, -2.75L, -2.5L, -1.5L, -0.5L, -0.25L, 0.25L, 0.5L, 1.5L, 2.5L, 2.75L, 100.5L };
	static const long long want[] = { -101LL, -3LL, -3LL, -2LL, -1LL, 0LL, 0LL, 1LL, 2LL, 3LL, 3LL, 101LL };
	volatile long double x;
	volatile long long r;
	int i, m;

	(void)m;
	for (i = 0; i < 12; i++) {
		x = in[i];
		r = llroundl(x);
		pc_check(r == want[i], "llroundl(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.llround.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
