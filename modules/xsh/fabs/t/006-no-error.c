/*
 * fabs() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fabs.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = -3.5;
	volatile double r;

	MT_START_E();
	r = fabs(x);
	MT_SAVE();
	(void)r;
	mt_noerr("fabs");
}

static void run_f(void)
{
	volatile float x = -3.5f;
	volatile float r;

	MT_START_E();
	r = fabsf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("fabsf");
}

static void run_l(void)
{
	volatile long double x = -3.5L;
	volatile long double r;

	MT_START_E();
	r = fabsl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("fabsl");
}

int main(void)
{
	pc_begin("XSH.fabs.006");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
