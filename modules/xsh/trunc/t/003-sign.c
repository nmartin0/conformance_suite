/*
 * The result of trunc() has the sign of x.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.trunc.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { -0.5, -0.25, 0.25, 0.5, -0.75, 0.75 };
	static const double want[] = { (-0.0), (-0.0), 0.0, 0.0, (-0.0), 0.0 };
	volatile double x, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = trunc(x);
		pc_check(MT_SAME(r, want[i]), "trunc(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { -0.5f, -0.25f, 0.25f, 0.5f, -0.75f, 0.75f };
	static const float want[] = { (-0.0f), (-0.0f), 0.0f, 0.0f, (-0.0f), 0.0f };
	volatile float x, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = truncf(x);
		pc_check(MT_SAME(r, want[i]), "truncf(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { -0.5L, -0.25L, 0.25L, 0.5L, -0.75L, 0.75L };
	static const long double want[] = { (-0.0L), (-0.0L), 0.0L, 0.0L, (-0.0L), 0.0L };
	volatile long double x, r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = truncl(x);
		pc_check(MT_SAME(r, want[i]), "truncl(%g) returned %g, expected %g", (double)x, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.trunc.003");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
