/*
 * llround() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llround.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile long long r;

	MT_START_E();
	r = llround(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llround");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile long long r;

	MT_START_E();
	r = llroundf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llroundf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long long r;

	MT_START_E();
	r = llroundl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llroundl");
}

int main(void)
{
	pc_begin("XSH.llround.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
