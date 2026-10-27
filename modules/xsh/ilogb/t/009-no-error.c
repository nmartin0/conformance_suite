/*
 * ilogb() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.009
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 8.0;
	volatile int r;

	MT_START_E();
	r = ilogb(x);
	MT_SAVE();
	(void)r;
	mt_noerr("ilogb");
}

static void run_f(void)
{
	volatile float x = 8.0f;
	volatile int r;

	MT_START_E();
	r = ilogbf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("ilogbf");
}

static void run_l(void)
{
	volatile long double x = 8.0L;
	volatile int r;

	MT_START_E();
	r = ilogbl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("ilogbl");
}

int main(void)
{
	pc_begin("XSH.ilogb.009");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
