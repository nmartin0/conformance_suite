/*
 * scalbn() overflow is a range error returning HUGE_VAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.002
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
		r = scalbn(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbn(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbn");
	}

	{
		x = -1.0;
		y = INT_MAX;
		MT_START();
		r = scalbn(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbn(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbn");
	}

	{
		x = DBL_MAX;
		y = INT_MAX;
		MT_START();
		r = scalbn(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "scalbn(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbn");
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
		r = scalbnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalbnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnf");
	}

	{
		x = -1.0f;
		y = INT_MAX;
		MT_START();
		r = scalbnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalbnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnf");
	}

	{
		x = FLT_MAX;
		y = INT_MAX;
		MT_START();
		r = scalbnf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "scalbnf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnf");
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
		r = scalbnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalbnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnl");
	}

	{
		x = -1.0L;
		y = INT_MAX;
		MT_START();
		r = scalbnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalbnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnl");
	}

	{
		x = LDBL_MAX;
		y = INT_MAX;
		MT_START();
		r = scalbnl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "scalbnl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("scalbnl");
	}
}

int main(void)
{
	pc_begin("XSH.scalbn.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
