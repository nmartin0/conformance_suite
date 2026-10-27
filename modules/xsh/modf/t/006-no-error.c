/*
 * modf() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.modf.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 3.75, r;
	double ip;

	MT_START_E();
	r = modf(x, &ip);
	MT_SAVE();
	(void)r;
	mt_noerr("modf");
}

static void run_f(void)
{
	volatile float x = 3.75f, r;
	float ip;

	MT_START_E();
	r = modff(x, &ip);
	MT_SAVE();
	(void)r;
	mt_noerr("modff");
}

static void run_l(void)
{
	volatile long double x = 3.75L, r;
	long double ip;

	MT_START_E();
	r = modfl(x, &ip);
	MT_SAVE();
	(void)r;
	mt_noerr("modfl");
}

int main(void)
{
	pc_begin("XSH.modf.006");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
