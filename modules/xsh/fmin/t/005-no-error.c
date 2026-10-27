/*
 * fmin() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmin.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 3.0;
	volatile double y = 5.0;
	volatile double r;

	MT_START_E();
	r = fmin(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmin");
}

static void run_f(void)
{
	volatile float x = 3.0f;
	volatile float y = 5.0f;
	volatile float r;

	MT_START_E();
	r = fminf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fminf");
}

static void run_l(void)
{
	volatile long double x = 3.0L;
	volatile long double y = 5.0L;
	volatile long double r;

	MT_START_E();
	r = fminl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fminl");
}

int main(void)
{
	pc_begin("XSH.fmin.005");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
