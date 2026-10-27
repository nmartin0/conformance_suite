/*
 * ilogb() of a zero, an infinity or a NaN is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.007
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile int r;

	{
		x = 0.0;
		MT_START();
		r = ilogb(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogb");
	}

	{
		x = (-0.0);
		MT_START();
		r = ilogb(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogb");
	}

	{
		x = (double)INFINITY;
		MT_START();
		r = ilogb(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogb");
	}

	{
		x = (-(double)INFINITY);
		MT_START();
		r = ilogb(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogb");
	}

	{
		x = (double)NAN;
		MT_START();
		r = ilogb(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogb");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile int r;

	{
		x = 0.0f;
		MT_START();
		r = ilogbf(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbf");
	}

	{
		x = (-0.0f);
		MT_START();
		r = ilogbf(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbf");
	}

	{
		x = (float)INFINITY;
		MT_START();
		r = ilogbf(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbf");
	}

	{
		x = (-(float)INFINITY);
		MT_START();
		r = ilogbf(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbf");
	}

	{
		x = (float)NAN;
		MT_START();
		r = ilogbf(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile int r;

	{
		x = 0.0L;
		MT_START();
		r = ilogbl(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbl");
	}

	{
		x = (-0.0L);
		MT_START();
		r = ilogbl(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbl");
	}

	{
		x = (long double)INFINITY;
		MT_START();
		r = ilogbl(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbl");
	}

	{
		x = (-(long double)INFINITY);
		MT_START();
		r = ilogbl(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbl");
	}

	{
		x = (long double)NAN;
		MT_START();
		r = ilogbl(x);
		MT_SAVE();
		(void)r;
		mt_domain("ilogbl");
	}
}

int main(void)
{
	pc_begin("XSH.ilogb.007");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
