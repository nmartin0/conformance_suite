/*
 * fmod() with a NaN operand returns a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { (double)NAN, 2.0, (double)NAN, (-(double)NAN) };
	static const double in2[] = { 2.0, (double)NAN, (double)NAN, 2.0 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmod(x, y);
		pc_check(isnan(r), "fmod(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_f(void)
{
	static const float in1[] = { (float)NAN, 2.0f, (float)NAN, (-(float)NAN) };
	static const float in2[] = { 2.0f, (float)NAN, (float)NAN, 2.0f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodf(x, y);
		pc_check(isnan(r), "fmodf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

static void run_l(void)
{
	static const long double in1[] = { (long double)NAN, 2.0L, (long double)NAN, (-(long double)NAN) };
	static const long double in2[] = { 2.0L, (long double)NAN, (long double)NAN, 2.0L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodl(x, y);
		pc_check(isnan(r), "fmodl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.fmod.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
