/*
 * fdim() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdim.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 5.0;
	volatile double y = 3.0;
	volatile double r;

	MT_START_E();
	r = fdim(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fdim");
}

static void run_f(void)
{
	volatile float x = 5.0f;
	volatile float y = 3.0f;
	volatile float r;

	MT_START_E();
	r = fdimf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fdimf");
}

static void run_l(void)
{
	volatile long double x = 5.0L;
	volatile long double y = 3.0L;
	volatile long double r;

	MT_START_E();
	r = fdiml(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fdiml");
}

int main(void)
{
	pc_begin("XSH.fdim.006");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
