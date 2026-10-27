/*
 * fmod(x, 0) is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.005
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
		r = fmod(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmod(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmod");
	}

	{
		x = -5.5;
		y = 0.0;
		MT_START();
		r = fmod(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmod(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmod");
	}

	{
		x = 0.0;
		y = 0.0;
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
		x = 5.5f;
		y = 0.0f;
		MT_START();
		r = fmodf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodf");
	}

	{
		x = -5.5f;
		y = 0.0f;
		MT_START();
		r = fmodf(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodf(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodf");
	}

	{
		x = 0.0f;
		y = 0.0f;
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
		x = 5.5L;
		y = 0.0L;
		MT_START();
		r = fmodl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodl");
	}

	{
		x = -5.5L;
		y = 0.0L;
		MT_START();
		r = fmodl(x, y);
		MT_SAVE();
		(void)r;
		pc_check(isnan(r), "fmodl(%g, %g) returned %g", (double)x, (double)y, (double)r);
		mt_domain("fmodl");
	}

	{
		x = 0.0L;
		y = 0.0L;
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
	pc_begin("XSH.fmod.005");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
