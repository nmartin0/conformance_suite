/*
 * remquo() with y zero or x infinite is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remquo.004
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 5.5, 0.0, (double)INFINITY, (-(double)INFINITY) };
	static const double in2[] = { 0.0, 0.0, 2.0, 2.0 };
	volatile double x, y, r;
	int i, q;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		MT_START();
		r = remquo(x, y, &q);
		MT_SAVE();
		pc_check(isnan(r), "remquo(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_domain("remquo");
	}
}

static void run_f(void)
{
	static const float in1[] = { 5.5f, 0.0f, (float)INFINITY, (-(float)INFINITY) };
	static const float in2[] = { 0.0f, 0.0f, 2.0f, 2.0f };
	volatile float x, y, r;
	int i, q;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		MT_START();
		r = remquof(x, y, &q);
		MT_SAVE();
		pc_check(isnan(r), "remquof(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_domain("remquof");
	}
}

static void run_l(void)
{
	static const long double in1[] = { 5.5L, 0.0L, (long double)INFINITY, (-(long double)INFINITY) };
	static const long double in2[] = { 0.0L, 0.0L, 2.0L, 2.0L };
	volatile long double x, y, r;
	int i, q;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		q = 0;
		MT_START();
		r = remquol(x, y, &q);
		MT_SAVE();
		pc_check(isnan(r), "remquol(%g, %g, &q) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_domain("remquol");
	}
}

int main(void)
{
	pc_begin("XSH.remquo.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
