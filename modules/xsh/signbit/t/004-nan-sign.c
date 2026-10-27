/*
 * signbit() sees the sign of a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.signbit.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y;
	volatile int a, b;

	x = (double)NAN;
	y = -x;
	a = signbit(x);
	b = signbit(y);
	pc_check((a != 0) != (b != 0), "signbit() of a NaN and of its negation agree (%d, %d) for double", a, b);
}

static void run_f(void)
{
	volatile float x, y;
	volatile int a, b;

	x = (float)NAN;
	y = -x;
	a = signbit(x);
	b = signbit(y);
	pc_check((a != 0) != (b != 0), "signbit() of a NaN and of its negation agree (%d, %d) for float", a, b);
}

static void run_l(void)
{
	volatile long double x, y;
	volatile int a, b;

	x = (long double)NAN;
	y = -x;
	a = signbit(x);
	b = signbit(y);
	pc_check((a != 0) != (b != 0), "signbit() of a NaN and of its negation agree (%d, %d) for long double", a, b);
}

int main(void)
{
	pc_begin("XSH.signbit.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
