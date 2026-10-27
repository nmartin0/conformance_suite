/*
 * fma() of an infinite product and an opposite infinity is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double y;
	volatile double z;
	volatile double r;

	{
		x = (double)INFINITY;
		y = 1.0;
		z = (-(double)INFINITY);
		MT_START();
		r = fma(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fma");
	}

	{
		x = (-(double)INFINITY);
		y = 1.0;
		z = (double)INFINITY;
		MT_START();
		r = fma(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fma");
	}

	{
		x = 1.0;
		y = (double)INFINITY;
		z = (-(double)INFINITY);
		MT_START();
		r = fma(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fma");
	}

	{
		x = (double)INFINITY;
		y = (double)INFINITY;
		z = (-(double)INFINITY);
		MT_START();
		r = fma(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fma");
	}

	{
		x = (double)INFINITY;
		y = -1.0;
		z = (double)INFINITY;
		MT_START();
		r = fma(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fma(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fma");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float y;
	volatile float z;
	volatile float r;

	{
		x = (float)INFINITY;
		y = 1.0f;
		z = (-(float)INFINITY);
		MT_START();
		r = fmaf(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmaf");
	}

	{
		x = (-(float)INFINITY);
		y = 1.0f;
		z = (float)INFINITY;
		MT_START();
		r = fmaf(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmaf");
	}

	{
		x = 1.0f;
		y = (float)INFINITY;
		z = (-(float)INFINITY);
		MT_START();
		r = fmaf(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmaf");
	}

	{
		x = (float)INFINITY;
		y = (float)INFINITY;
		z = (-(float)INFINITY);
		MT_START();
		r = fmaf(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmaf");
	}

	{
		x = (float)INFINITY;
		y = -1.0f;
		z = (float)INFINITY;
		MT_START();
		r = fmaf(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmaf(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmaf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double y;
	volatile long double z;
	volatile long double r;

	{
		x = (long double)INFINITY;
		y = 1.0L;
		z = (-(long double)INFINITY);
		MT_START();
		r = fmal(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmal");
	}

	{
		x = (-(long double)INFINITY);
		y = 1.0L;
		z = (long double)INFINITY;
		MT_START();
		r = fmal(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmal");
	}

	{
		x = 1.0L;
		y = (long double)INFINITY;
		z = (-(long double)INFINITY);
		MT_START();
		r = fmal(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmal");
	}

	{
		x = (long double)INFINITY;
		y = (long double)INFINITY;
		z = (-(long double)INFINITY);
		MT_START();
		r = fmal(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmal");
	}

	{
		x = (long double)INFINITY;
		y = -1.0L;
		z = (long double)INFINITY;
		MT_START();
		r = fmal(x, y, z);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmal(%g, %g, %g) returned %g", (double)x, (double)y, (double)z, (double)r);
		mt_domain("fmal");
	}
}

int main(void)
{
	pc_begin("XSH.fma.005");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
