/*
 * nextafter() of equal zeros returns y with its sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (-0.0), 0.0 };
	static const double in2[] = { 0.0, (-0.0) };
	static const double want[] = { 0.0, (-0.0) };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafter(x, y);
		pc_check(MT_SAME(r, want[i]), "nextafter(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { (-0.0f), 0.0f };
	static const float in2[] = { 0.0f, (-0.0f) };
	static const float want[] = { 0.0f, (-0.0f) };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterf(x, y);
		pc_check(MT_SAME(r, want[i]), "nextafterf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (-0.0L), 0.0L };
	static const long double in2[] = { 0.0L, (-0.0L) };
	static const long double want[] = { 0.0L, (-0.0L) };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 2; i++) {
		x = in1[i];
		y = in2[i];
		r = nextafterl(x, y);
		pc_check(MT_SAME(r, want[i]), "nextafterl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.003");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
