/*
 * lround() of a NaN is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lround.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long r;

	{
		x = (double)NAN;
		MT_START();
		r = lround(x);
		MT_SAVE();
		(void)r;
		mt_domain("lround");
	}

	{
		x = (-(double)NAN);
		MT_START();
		r = lround(x);
		MT_SAVE();
		(void)r;
		mt_domain("lround");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile long r;

	{
		x = (float)NAN;
		MT_START();
		r = lroundf(x);
		MT_SAVE();
		(void)r;
		mt_domain("lroundf");
	}

	{
		x = (-(float)NAN);
		MT_START();
		r = lroundf(x);
		MT_SAVE();
		(void)r;
		mt_domain("lroundf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long r;

	{
		x = (long double)NAN;
		MT_START();
		r = lroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("lroundl");
	}

	{
		x = (-(long double)NAN);
		MT_START();
		r = lroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("lroundl");
	}
}

int main(void)
{
	pc_begin("XSH.lround.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
