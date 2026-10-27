/*
 * remainder() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remainder.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 5.5;
	volatile double y = 2.0;
	volatile double r;

	MT_START_E();
	r = remainder(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("remainder");
}

static void run_f(void)
{
	volatile float x = 5.5f;
	volatile float y = 2.0f;
	volatile float r;

	MT_START_E();
	r = remainderf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("remainderf");
}

static void run_l(void)
{
	volatile long double x = 5.5L;
	volatile long double y = 2.0L;
	volatile long double r;

	MT_START_E();
	r = remainderl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("remainderl");
}

int main(void)
{
	pc_begin("XSH.remainder.006");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
