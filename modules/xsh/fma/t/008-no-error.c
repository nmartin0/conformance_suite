/*
 * fma() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.0;
	volatile double y = 3.0;
	volatile double z = 4.0;
	volatile double r;

	MT_START_E();
	r = fma(x, y, z);
	MT_SAVE();
	(void)r;
	mt_noerr("fma");
}

static void run_f(void)
{
	volatile float x = 2.0f;
	volatile float y = 3.0f;
	volatile float z = 4.0f;
	volatile float r;

	MT_START_E();
	r = fmaf(x, y, z);
	MT_SAVE();
	(void)r;
	mt_noerr("fmaf");
}

static void run_l(void)
{
	volatile long double x = 2.0L;
	volatile long double y = 3.0L;
	volatile long double z = 4.0L;
	volatile long double r;

	MT_START_E();
	r = fmal(x, y, z);
	MT_SAVE();
	(void)r;
	mt_noerr("fmal");
}

int main(void)
{
	pc_begin("XSH.fma.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
