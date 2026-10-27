/*
 * round() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.round.007
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile double r;

	MT_START_E();
	r = round(x);
	MT_SAVE();
	(void)r;
	mt_noerr("round");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile float r;

	MT_START_E();
	r = roundf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("roundf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long double r;

	MT_START_E();
	r = roundl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("roundl");
}

int main(void)
{
	pc_begin("XSH.round.007");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
