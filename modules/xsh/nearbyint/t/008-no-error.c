/*
 * nearbyint() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nearbyint.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile double r;

	MT_START_E();
	r = nearbyint(x);
	MT_SAVE();
	(void)r;
	mt_noerr("nearbyint");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile float r;

	MT_START_E();
	r = nearbyintf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("nearbyintf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long double r;

	MT_START_E();
	r = nearbyintl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("nearbyintl");
}

int main(void)
{
	pc_begin("XSH.nearbyint.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
