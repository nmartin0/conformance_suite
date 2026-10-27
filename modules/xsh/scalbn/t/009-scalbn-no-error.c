/*
 * scalbn() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.scalbn.009
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 1.5;
	volatile int y = 3;
	volatile double r;

	MT_START_E();
	r = scalbn(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalbn");
}

static void run_f(void)
{
	volatile float x = 1.5f;
	volatile int y = 3;
	volatile float r;

	MT_START_E();
	r = scalbnf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalbnf");
}

static void run_l(void)
{
	volatile long double x = 1.5L;
	volatile int y = 3;
	volatile long double r;

	MT_START_E();
	r = scalbnl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("scalbnl");
}

int main(void)
{
	pc_begin("XSH.scalbn.009");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
