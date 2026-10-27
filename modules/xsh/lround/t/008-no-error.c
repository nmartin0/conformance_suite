/*
 * lround() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lround.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile long r;

	MT_START_E();
	r = lround(x);
	MT_SAVE();
	(void)r;
	mt_noerr("lround");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile long r;

	MT_START_E();
	r = lroundf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("lroundf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long r;

	MT_START_E();
	r = lroundl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("lroundl");
}

int main(void)
{
	pc_begin("XSH.lround.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
