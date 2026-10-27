/*
 * remquo() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remquo.005
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 5.5, y = 2.0, r;
	int q = 0;

	MT_START_E();
	r = remquo(x, y, &q);
	MT_SAVE();
	(void)r;
	mt_noerr("remquo");
}

static void run_f(void)
{
	volatile float x = 5.5f, y = 2.0f, r;
	int q = 0;

	MT_START_E();
	r = remquof(x, y, &q);
	MT_SAVE();
	(void)r;
	mt_noerr("remquof");
}

static void run_l(void)
{
	volatile long double x = 5.5L, y = 2.0L, r;
	int q = 0;

	MT_START_E();
	r = remquol(x, y, &q);
	MT_SAVE();
	(void)r;
	mt_noerr("remquol");
}

int main(void)
{
	pc_begin("XSH.remquo.005");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
