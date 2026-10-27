/*
 * fmod() of a finite x and an infinite y returns x.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.008
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 5.5, -5.5, 5.5, -5.5 };
	static const double in2[] = { (double)INFINITY, (double)INFINITY, (-(double)INFINITY), (-(double)INFINITY) };
	static const double want[] = { 5.5, -5.5, 5.5, -5.5 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmod(x, y);
		pc_check(MT_SAME(r, want[i]), "fmod(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 5.5f, -5.5f, 5.5f, -5.5f };
	static const float in2[] = { (float)INFINITY, (float)INFINITY, (-(float)INFINITY), (-(float)INFINITY) };
	static const float want[] = { 5.5f, -5.5f, 5.5f, -5.5f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodf(x, y);
		pc_check(MT_SAME(r, want[i]), "fmodf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 5.5L, -5.5L, 5.5L, -5.5L };
	static const long double in2[] = { (long double)INFINITY, (long double)INFINITY, (-(long double)INFINITY), (-(long double)INFINITY) };
	static const long double want[] = { 5.5L, -5.5L, 5.5L, -5.5L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 4; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodl(x, y);
		pc_check(MT_SAME(r, want[i]), "fmodl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fmod.008");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
