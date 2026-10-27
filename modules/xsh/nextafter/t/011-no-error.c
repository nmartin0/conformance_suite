/*
 * nextafter() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.011
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 1.0;
	volatile double y = 2.0;
	volatile double r;

	MT_START_E();
	r = nextafter(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("nextafter");
}

static void run_f(void)
{
	volatile float x = 1.0f;
	volatile float y = 2.0f;
	volatile float r;

	MT_START_E();
	r = nextafterf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("nextafterf");
}

static void run_l(void)
{
	volatile long double x = 1.0L;
	volatile long double y = 2.0L;
	volatile long double r;

	MT_START_E();
	r = nextafterl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("nextafterl");
}

int main(void)
{
	pc_begin("XSH.nextafter.011");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
