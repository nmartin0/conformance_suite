/*
 * ceil() of ordinary values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ceil.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -100.0625, -3.5, -2.75, -2.5, -1.5, -1.25, -0.75, 0.75, 1.25, 1.5, 2.5, 2.75, 3.0, -3.0, 3.5, 100.0625 };
	static const double want[] = { -100.0, -3.0, -2.0, -2.0, -1.0, -1.0, 0.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, -3.0, 4.0, 101.0 };
	volatile double x, r;
	int i;

	for (i = 0; i < 16; i++) {
		x = in[i];
		r = ceil(x);
		pc_check(r == want[i], "ceil(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { -100.0625f, -3.5f, -2.75f, -2.5f, -1.5f, -1.25f, -0.75f, 0.75f, 1.25f, 1.5f, 2.5f, 2.75f, 3.0f, -3.0f, 3.5f, 100.0625f };
	static const float want[] = { -100.0f, -3.0f, -2.0f, -2.0f, -1.0f, -1.0f, 0.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f, 3.0f, -3.0f, 4.0f, 101.0f };
	volatile float x, r;
	int i;

	for (i = 0; i < 16; i++) {
		x = in[i];
		r = ceilf(x);
		pc_check(r == want[i], "ceilf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { -100.0625L, -3.5L, -2.75L, -2.5L, -1.5L, -1.25L, -0.75L, 0.75L, 1.25L, 1.5L, 2.5L, 2.75L, 3.0L, -3.0L, 3.5L, 100.0625L };
	static const long double want[] = { -100.0L, -3.0L, -2.0L, -2.0L, -1.0L, -1.0L, 0.0L, 1.0L, 2.0L, 2.0L, 3.0L, 3.0L, 3.0L, -3.0L, 4.0L, 101.0L };
	volatile long double x, r;
	int i;

	for (i = 0; i < 16; i++) {
		x = in[i];
		r = ceill(x);
		pc_check(r == want[i], "ceill(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.ceil.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
