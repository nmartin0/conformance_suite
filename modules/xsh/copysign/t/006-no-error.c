/*
 * copysign() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.copysign.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 3.0;
	volatile double y = -1.0;
	volatile double r;

	MT_START_E();
	r = copysign(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("copysign");
}

static void run_f(void)
{
	volatile float x = 3.0f;
	volatile float y = -1.0f;
	volatile float r;

	MT_START_E();
	r = copysignf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("copysignf");
}

static void run_l(void)
{
	volatile long double x = 3.0L;
	volatile long double y = -1.0L;
	volatile long double r;

	MT_START_E();
	r = copysignl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("copysignl");
}

int main(void)
{
	pc_begin("XSH.copysign.006");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
