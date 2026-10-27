/*
 * llround() of a NaN is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llround.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long long r;

	{
		x = (double)NAN;
		MT_START();
		r = llround(x);
		MT_SAVE();
		(void)r;
		mt_domain("llround");
	}

	{
		x = (-(double)NAN);
		MT_START();
		r = llround(x);
		MT_SAVE();
		(void)r;
		mt_domain("llround");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile long long r;

	{
		x = (float)NAN;
		MT_START();
		r = llroundf(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundf");
	}

	{
		x = (-(float)NAN);
		MT_START();
		r = llroundf(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long long r;

	{
		x = (long double)NAN;
		MT_START();
		r = llroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundl");
	}

	{
		x = (-(long double)NAN);
		MT_START();
		r = llroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundl");
	}
}

int main(void)
{
	pc_begin("XSH.llround.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
