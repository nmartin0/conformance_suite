/*
 * scalbln() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.018
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 1.5;
	volatile long y = 3;
	volatile double r;

	MT_START_E();
	r = scalbln(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalbln");
}

static void run_f(void)
{
	volatile float x = 1.5f;
	volatile long y = 3;
	volatile float r;

	MT_START_E();
	r = scalblnf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalblnf");
}

static void run_l(void)
{
	volatile long double x = 1.5L;
	volatile long y = 3;
	volatile long double r;

	MT_START_E();
	r = scalblnl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalblnl");
}

int main(void)
{
	pc_begin("XSH.scalbn.018");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
