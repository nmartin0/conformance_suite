/*
 * nextafter() of the largest value toward infinity is an overflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double r;

	{
		x = DBL_MAX;
		y = HUGE_VAL;
		MT_START();
		r = nextafter(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "nextafter(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafter");
	}

	{
		x = (-DBL_MAX);
		y = (-HUGE_VAL);
		MT_START();
		r = nextafter(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VAL : -HUGE_VAL), "nextafter(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafter");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float y;
	volatile float r;

	{
		x = FLT_MAX;
		y = HUGE_VALF;
		MT_START();
		r = nextafterf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "nextafterf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafterf");
	}

	{
		x = (-FLT_MAX);
		y = (-HUGE_VALF);
		MT_START();
		r = nextafterf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALF : -HUGE_VALF), "nextafterf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafterf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double y;
	volatile long double r;

	{
		x = LDBL_MAX;
		y = HUGE_VALL;
		MT_START();
		r = nextafterl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "nextafterl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafterl");
	}

	{
		x = (-LDBL_MAX);
		y = (-HUGE_VALL);
		MT_START();
		r = nextafterl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r == (x > 0 ? HUGE_VALL : -HUGE_VALL), "nextafterl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_overflow("nextafterl");
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.005");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
