/*
 * ldexp() overflow is a range error returning HUGE_VAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldexp.002
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile int y;
	volatile double r;

	{
		x = 1.0;
		y = INT_MAX;
		MT_START();
		r = ldexp(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "ldexp(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexp");
	}

	{
		x = -1.0;
		y = INT_MAX;
		MT_START();
		r = ldexp(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "ldexp(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexp");
	}

	{
		x = DBL_MAX;
		y = 1;
		MT_START();
		r = ldexp(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "ldexp(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexp");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile int y;
	volatile float r;

	{
		x = 1.0f;
		y = INT_MAX;
		MT_START();
		r = ldexpf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "ldexpf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpf");
	}

	{
		x = -1.0f;
		y = INT_MAX;
		MT_START();
		r = ldexpf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "ldexpf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpf");
	}

	{
		x = FLT_MAX;
		y = 1;
		MT_START();
		r = ldexpf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "ldexpf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile int y;
	volatile long double r;

	{
		x = 1.0L;
		y = INT_MAX;
		MT_START();
		r = ldexpl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "ldexpl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpl");
	}

	{
		x = -1.0L;
		y = INT_MAX;
		MT_START();
		r = ldexpl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "ldexpl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpl");
	}

	{
		x = LDBL_MAX;
		y = 1;
		MT_START();
		r = ldexpl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "ldexpl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("ldexpl");
	}
}

int main(void)
{
	pc_begin("XSH.ldexp.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
