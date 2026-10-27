/*
 * scalbln() overflow is a range error returning HUGE_VAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.011
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long y;
	volatile double r;

	{
		x = 1.0;
		y = LONG_MAX;
		MT_START();
		r = scalbln(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbln(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbln");
	}

	{
		x = -1.0;
		y = LONG_MAX;
		MT_START();
		r = scalbln(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbln(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbln");
	}

	{
		x = DBL_MAX;
		y = LONG_MAX;
		MT_START();
		r = scalbln(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbln(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbln");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile long y;
	volatile float r;

	{
		x = 1.0f;
		y = LONG_MAX;
		MT_START();
		r = scalblnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalblnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnf");
	}

	{
		x = -1.0f;
		y = LONG_MAX;
		MT_START();
		r = scalblnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalblnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnf");
	}

	{
		x = FLT_MAX;
		y = LONG_MAX;
		MT_START();
		r = scalblnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalblnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long y;
	volatile long double r;

	{
		x = 1.0L;
		y = LONG_MAX;
		MT_START();
		r = scalblnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalblnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnl");
	}

	{
		x = -1.0L;
		y = LONG_MAX;
		MT_START();
		r = scalblnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalblnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnl");
	}

	{
		x = LDBL_MAX;
		y = LONG_MAX;
		MT_START();
		r = scalblnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalblnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalblnl");
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.011");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
