/*
 * nextafter() with a subnormal or zero result is a range error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double r;

	{
		x = DBL_MIN;
		y = 0.0;
		MT_START();
		r = nextafter(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < DBL_MIN, "nextafter(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafter");
	}

	{
		x = 0.0;
		y = 1.0;
		MT_START();
		r = nextafter(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < DBL_MIN, "nextafter(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafter");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float y;
	volatile float r;

	{
		x = FLT_MIN;
		y = 0.0f;
		MT_START();
		r = nextafterf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < FLT_MIN, "nextafterf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafterf");
	}

	{
		x = 0.0f;
		y = 1.0f;
		MT_START();
		r = nextafterf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < FLT_MIN, "nextafterf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafterf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double y;
	volatile long double r;

	{
		x = LDBL_MIN;
		y = 0.0L;
		MT_START();
		r = nextafterl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < LDBL_MIN, "nextafterl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafterl");
	}

	{
		x = 0.0L;
		y = 1.0L;
		MT_START();
		r = nextafterl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(r >= 0 && r < LDBL_MIN, "nextafterl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_underflow("nextafterl");
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
