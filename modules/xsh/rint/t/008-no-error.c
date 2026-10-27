/*
 * rint() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rint.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile double r;

	MT_START_E();
	r = rint(x);
	MT_SAVE();
	(void)r;
	mt_noerr("rint");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile float r;

	MT_START_E();
	r = rintf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("rintf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long double r;

	MT_START_E();
	r = rintl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("rintl");
}

int main(void)
{
	pc_begin("XSH.rint.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
