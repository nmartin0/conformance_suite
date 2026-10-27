/*
 * fmod() computes x - i*y with the sign of x and a smaller magnitude than y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.001
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 5.5, -5.5, 5.5, -5.5, 1.5, -1.5, 6.0, 7.0, 0.75, 100.0 };
	static const double in2[] = { 2.0, 2.0, -2.0, -2.0, 4.0, 4.0, 3.0, 2.5, 0.25, 7.0 };
	static const double want[] = { 1.5, -1.5, 1.5, -1.5, 1.5, -1.5, 0.0, 2.0, 0.0, 2.0 };
	volatile double x, y, r;
	int i;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		r = fmod(x, y);
		pc_check(r == want[i], "fmod(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 5.5f, -5.5f, 5.5f, -5.5f, 1.5f, -1.5f, 6.0f, 7.0f, 0.75f, 100.0f };
	static const float in2[] = { 2.0f, 2.0f, -2.0f, -2.0f, 4.0f, 4.0f, 3.0f, 2.5f, 0.25f, 7.0f };
	static const float want[] = { 1.5f, -1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 0.0f, 2.0f, 0.0f, 2.0f };
	volatile float x, y, r;
	int i;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodf(x, y);
		pc_check(r == want[i], "fmodf(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 5.5L, -5.5L, 5.5L, -5.5L, 1.5L, -1.5L, 6.0L, 7.0L, 0.75L, 100.0L };
	static const long double in2[] = { 2.0L, 2.0L, -2.0L, -2.0L, 4.0L, 4.0L, 3.0L, 2.5L, 0.25L, 7.0L };
	static const long double want[] = { 1.5L, -1.5L, 1.5L, -1.5L, 1.5L, -1.5L, 0.0L, 2.0L, 0.0L, 2.0L };
	volatile long double x, y, r;
	int i;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		r = fmodl(x, y);
		pc_check(r == want[i], "fmodl(%g, %g) returned %g, expected %g", (double)x, (double)y, (double)r, (double)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.fmod.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
