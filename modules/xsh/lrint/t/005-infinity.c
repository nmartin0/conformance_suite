/*
 * lrint() of an infinity is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lrint.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long r;

	{
		x = (double)INFINITY;
		MT_START();
		r = lrint(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrint");
	}

	{
		x = (-(double)INFINITY);
		MT_START();
		r = lrint(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrint");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile long r;

	{
		x = (float)INFINITY;
		MT_START();
		r = lrintf(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrintf");
	}

	{
		x = (-(float)INFINITY);
		MT_START();
		r = lrintf(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrintf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long r;

	{
		x = (long double)INFINITY;
		MT_START();
		r = lrintl(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrintl");
	}

	{
		x = (-(long double)INFINITY);
		MT_START();
		r = lrintl(x);
		MT_SAVE();
		(void)r;
		mt_domain("lrintl");
	}
}

int main(void)
{
	pc_begin("XSH.lrint.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
