/*
 * llround() of an infinity is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llround.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long long r;

	{
		x = (double)INFINITY;
		MT_START();
		r = llround(x);
		MT_SAVE();
		(void)r;
		mt_domain("llround");
	}

	{
		x = (-(double)INFINITY);
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
		x = (float)INFINITY;
		MT_START();
		r = llroundf(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundf");
	}

	{
		x = (-(float)INFINITY);
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
		x = (long double)INFINITY;
		MT_START();
		r = llroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundl");
	}

	{
		x = (-(long double)INFINITY);
		MT_START();
		r = llroundl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llroundl");
	}
}

int main(void)
{
	pc_begin("XSH.llround.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
