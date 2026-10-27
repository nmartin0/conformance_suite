/*
 * fmod() of ordinary values reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.009
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 5.5;
	volatile double y = 2.0;
	volatile double r;

	MT_START_E();
	r = fmod(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmod");
}

static void run_f(void)
{
	volatile float x = 5.5f;
	volatile float y = 2.0f;
	volatile float r;

	MT_START_E();
	r = fmodf(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmodf");
}

static void run_l(void)
{
	volatile long double x = 5.5L;
	volatile long double y = 2.0L;
	volatile long double r;

	MT_START_E();
	r = fmodl(x, y);
	MT_SAVE();
	(void)r;
	mt_noerr("fmodl");
}

int main(void)
{
	pc_begin("XSH.fmod.009");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
