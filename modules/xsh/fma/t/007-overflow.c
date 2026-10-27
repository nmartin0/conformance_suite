/*
 * fma() overflow is a range error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.007
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, z, r;

	x = DBL_MAX;
	y = 2;
	z = 0;
	MT_START();
	r = fma(x, y, z);
	MT_SAVE();
	pc_check(r == HUGE_VAL, "fma(MAX, 2, 0) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("fma(MAX, 2, 0)");
	x = -DBL_MAX;
	MT_START();
	r = fma(x, y, z);
	MT_SAVE();
	pc_check(r == -HUGE_VAL, "fma(-MAX, 2, 0) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("fma(-MAX, 2, 0)");
}

static void run_f(void)
{
	volatile float x, y, z, r;

	x = FLT_MAX;
	y = 2;
	z = 0;
	MT_START();
	r = fmaf(x, y, z);
	MT_SAVE();
	pc_check(r == HUGE_VALF, "fmaf(MAX, 2, 0) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("fmaf(MAX, 2, 0)");
	x = -FLT_MAX;
	MT_START();
	r = fmaf(x, y, z);
	MT_SAVE();
	pc_check(r == -HUGE_VALF, "fmaf(-MAX, 2, 0) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("fmaf(-MAX, 2, 0)");
}

static void run_l(void)
{
	volatile long double x, y, z, r;

	x = LDBL_MAX;
	y = 2;
	z = 0;
	MT_START();
	r = fmal(x, y, z);
	MT_SAVE();
	pc_check(r == HUGE_VALL, "fmal(MAX, 2, 0) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("fmal(MAX, 2, 0)");
	x = -LDBL_MAX;
	MT_START();
	r = fmal(x, y, z);
	MT_SAVE();
	pc_check(r == -HUGE_VALL, "fmal(-MAX, 2, 0) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("fmal(-MAX, 2, 0)");
}

int main(void)
{
	pc_begin("XSH.fma.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
