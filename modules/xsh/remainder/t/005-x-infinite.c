/*
 * remainder() of an infinite x is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remainder.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double r;

	{
		x = (double)INFINITY;
		y = 2.0;
		MT_START();
		r = remainder(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainder(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainder");
	}

	{
		x = (-(double)INFINITY);
		y = 2.0;
		MT_START();
		r = remainder(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainder(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainder");
	}

	{
		x = (double)INFINITY;
		y = (double)INFINITY;
		MT_START();
		r = remainder(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainder(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainder");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float y;
	volatile float r;

	{
		x = (float)INFINITY;
		y = 2.0f;
		MT_START();
		r = remainderf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderf");
	}

	{
		x = (-(float)INFINITY);
		y = 2.0f;
		MT_START();
		r = remainderf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderf");
	}

	{
		x = (float)INFINITY;
		y = (float)INFINITY;
		MT_START();
		r = remainderf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double y;
	volatile long double r;

	{
		x = (long double)INFINITY;
		y = 2.0L;
		MT_START();
		r = remainderl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderl");
	}

	{
		x = (-(long double)INFINITY);
		y = 2.0L;
		MT_START();
		r = remainderl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderl");
	}

	{
		x = (long double)INFINITY;
		y = (long double)INFINITY;
		MT_START();
		r = remainderl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderl");
	}
}

int main(void)
{
	pc_begin("XSH.remainder.005");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
