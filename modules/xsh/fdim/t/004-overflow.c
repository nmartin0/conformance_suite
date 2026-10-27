/*
 * fdim() overflow is a range error returning HUGE_VAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdim.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double r;

	{
		x = DBL_MAX;
		y = (-DBL_MAX);
		MT_START();
		r = fdim(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == HUGE_VAL, "fdim(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("fdim");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float y;
	volatile float r;

	{
		x = FLT_MAX;
		y = (-FLT_MAX);
		MT_START();
		r = fdimf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == HUGE_VALF, "fdimf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("fdimf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double y;
	volatile long double r;

	{
		x = LDBL_MAX;
		y = (-LDBL_MAX);
		MT_START();
		r = fdiml(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == HUGE_VALL, "fdiml(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("fdiml");
	}
}

int main(void)
{
	pc_begin("XSH.fdim.004");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
