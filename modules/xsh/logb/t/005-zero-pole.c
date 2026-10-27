/*
 * logb() of a zero is a pole error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile double r;

	{
		x = 0.0;
		MT_START();
		r = logb(x);
		MT_SAVE();
		(void)r;
		mt_pole("logb");
	}

	{
		x = (-0.0);
		MT_START();
		r = logb(x);
		MT_SAVE();
		(void)r;
		mt_pole("logb");
	}
}

static void run_f(void)
{
	volatile float x;
	volatile float r;

	{
		x = 0.0f;
		MT_START();
		r = logbf(x);
		MT_SAVE();
		(void)r;
		mt_pole("logbf");
	}

	{
		x = (-0.0f);
		MT_START();
		r = logbf(x);
		MT_SAVE();
		(void)r;
		mt_pole("logbf");
	}
}

static void run_l(void)
{
	volatile long double x;
	volatile long double r;

	{
		x = 0.0L;
		MT_START();
		r = logbl(x);
		MT_SAVE();
		(void)r;
		mt_pole("logbl");
	}

	{
		x = (-0.0L);
		MT_START();
		r = logbl(x);
		MT_SAVE();
		(void)r;
		mt_pole("logbl");
	}
}

int main(void)
{
	pc_begin("XSH.logb.005");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
