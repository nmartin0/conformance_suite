/*
 * logb() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.logb.009
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 10.0;
	volatile double r;

	MT_START_E();
	r = logb(x);
	MT_SAVE();
	(void)r;
	mt_noerr("logb");
}

static void run_f(void)
{
	volatile float x = 10.0f;
	volatile float r;

	MT_START_E();
	r = logbf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("logbf");
}

static void run_l(void)
{
	volatile long double x = 10.0L;
	volatile long double r;

	MT_START_E();
	r = logbl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("logbl");
}

int main(void)
{
	pc_begin("XSH.logb.009");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
