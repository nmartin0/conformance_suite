/*
 * fmod() of an infinite x is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.006
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
		r = fmod(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmod(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmod");
	}

	{
		x = (-(double)INFINITY);
		y = 2.0;
		MT_START();
		r = fmod(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmod(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmod");
	}

	{
		x = (double)INFINITY;
		y = (double)INFINITY;
		MT_START();
		r = fmod(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmod(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmod");
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
		r = fmodf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodf");
	}

	{
		x = (-(float)INFINITY);
		y = 2.0f;
		MT_START();
		r = fmodf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodf");
	}

	{
		x = (float)INFINITY;
		y = (float)INFINITY;
		MT_START();
		r = fmodf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodf");
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
		r = fmodl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodl");
	}

	{
		x = (-(long double)INFINITY);
		y = 2.0L;
		MT_START();
		r = fmodl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodl");
	}

	{
		x = (long double)INFINITY;
		y = (long double)INFINITY;
		MT_START();
		r = fmodl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodl");
	}
}

int main(void)
{
	pc_begin("XSH.fmod.006");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
