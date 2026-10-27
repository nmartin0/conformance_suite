/*
 * fmax() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmax.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 3.0;
	volatile double y = 5.0;
	volatile double r;

	MT_START_E();
	r = fmax(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmax");
}

static void run_f(void)
{
	volatile float x = 3.0f;
	volatile float y = 5.0f;
	volatile float r;

	MT_START_E();
	r = fmaxf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmaxf");
}

static void run_l(void)
{
	volatile long double x = 3.0L;
	volatile long double y = 5.0L;
	volatile long double r;

	MT_START_E();
	r = fmaxl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmaxl");
}

int main(void)
{
	pc_begin("XSH.fmax.005");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
