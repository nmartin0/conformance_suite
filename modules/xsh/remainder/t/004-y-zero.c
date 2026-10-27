/*
 * remainder(x, 0) is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remainder.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double r;

	{
		x = 5.5;
		y = 0.0;
		MT_START();
		r = remainder(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainder(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainder");
	}

	{
		x = -5.5;
		y = 0.0;
		MT_START();
		r = remainder(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainder(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainder");
	}

	{
		x = 0.0;
		y = 0.0;
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
		x = 5.5f;
		y = 0.0f;
		MT_START();
		r = remainderf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderf");
	}

	{
		x = -5.5f;
		y = 0.0f;
		MT_START();
		r = remainderf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderf");
	}

	{
		x = 0.0f;
		y = 0.0f;
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
		x = 5.5L;
		y = 0.0L;
		MT_START();
		r = remainderl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderl");
	}

	{
		x = -5.5L;
		y = 0.0L;
		MT_START();
		r = remainderl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "remainderl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("remainderl");
	}

	{
		x = 0.0L;
		y = 0.0L;
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
	pc_begin("XSH.remainder.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
