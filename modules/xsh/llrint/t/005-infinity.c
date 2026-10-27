/*
 * llrint() of an infinity is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llrint.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long long r;

	{
		x = (double)INFINITY;
		MT_START();
		r = llrint(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrint");
	}

	{
		x = (-(double)INFINITY);
		MT_START();
		r = llrint(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrint");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile long long r;

	{
		x = (float)INFINITY;
		MT_START();
		r = llrintf(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrintf");
	}

	{
		x = (-(float)INFINITY);
		MT_START();
		r = llrintf(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrintf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long long r;

	{
		x = (long double)INFINITY;
		MT_START();
		r = llrintl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrintl");
	}

	{
		x = (-(long double)INFINITY);
		MT_START();
		r = llrintl(x);
		MT_SAVE();
		(void)r;
		mt_domain("llrintl");
	}
}

int main(void)
{
	pc_begin("XSH.llrint.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
