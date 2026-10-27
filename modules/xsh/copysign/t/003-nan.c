/*
 * copysign() carries the sign of a NaN operand.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.003
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, r;

	x = (double)NAN;
	y = -1.0;
	r = copysign(x, y);
	pc_check(isnan(r) && signbit(r) != 0, "copysign(NaN, -1) returned %g without a NaN carrying the sign bit", (double)r);
	y = 1.0;
	r = copysign(x, y);
	pc_check(isnan(r) && signbit(r) == 0, "copysign(NaN, 1) returned %g without a NaN with the sign bit clear", (double)r);
	x = 1.0;
	y = (double)NAN;
	r = copysign(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0 - 2.0 * (signbit(y) != 0), "copysign(1, NaN) returned %g, whose sign differs from the NaN's", (double)r);
	y = -(double)NAN;
	r = copysign(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0 - 2.0 * (signbit(y) != 0), "copysign(1, -NaN) returned %g, whose sign differs from the NaN's", (double)r);
}

static void run_f(void)
{
	volatile float x, y, r;

	x = (float)NAN;
	y = -1.0f;
	r = copysignf(x, y);
	pc_check(isnan(r) && signbit(r) != 0, "copysignf(NaN, -1) returned %g without a NaN carrying the sign bit", (double)r);
	y = 1.0f;
	r = copysignf(x, y);
	pc_check(isnan(r) && signbit(r) == 0, "copysignf(NaN, 1) returned %g without a NaN with the sign bit clear", (double)r);
	x = 1.0f;
	y = (float)NAN;
	r = copysignf(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0f - 2.0f * (signbit(y) != 0), "copysignf(1, NaN) returned %g, whose sign differs from the NaN's", (double)r);
	y = -(float)NAN;
	r = copysignf(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0f - 2.0f * (signbit(y) != 0), "copysignf(1, -NaN) returned %g, whose sign differs from the NaN's", (double)r);
}

static void run_l(void)
{
	volatile long double x, y, r;

	x = (long double)NAN;
	y = -1.0L;
	r = copysignl(x, y);
	pc_check(isnan(r) && signbit(r) != 0, "copysignl(NaN, -1) returned %g without a NaN carrying the sign bit", (double)r);
	y = 1.0L;
	r = copysignl(x, y);
	pc_check(isnan(r) && signbit(r) == 0, "copysignl(NaN, 1) returned %g without a NaN with the sign bit clear", (double)r);
	x = 1.0L;
	y = (long double)NAN;
	r = copysignl(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0L - 2.0L * (signbit(y) != 0), "copysignl(1, NaN) returned %g, whose sign differs from the NaN's", (double)r);
	y = -(long double)NAN;
	r = copysignl(x, y);
	pc_check((signbit(r) != 0) == (signbit(y) != 0) && r == 1.0L - 2.0L * (signbit(y) != 0), "copysignl(1, -NaN) returned %g, whose sign differs from the NaN's", (double)r);
}

int main(void)
{
	pc_begin("XSH.copysign.003");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
