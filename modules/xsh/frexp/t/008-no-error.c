/*
 * frexp() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 8.0, r;
	int e;

	MT_START_E();
	r = frexp(x, &e);
	MT_SAVE();
	(void)r;
	mt_noerr("frexp");
}

static void run_f(void)
{
	volatile float x = 8.0f, r;
	int e;

	MT_START_E();
	r = frexpf(x, &e);
	MT_SAVE();
	(void)r;
	mt_noerr("frexpf");
}

static void run_l(void)
{
	volatile long double x = 8.0L, r;
	int e;

	MT_START_E();
	r = frexpl(x, &e);
	MT_SAVE();
	(void)r;
	mt_noerr("frexpl");
}

int main(void)
{
	pc_begin("XSH.frexp.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
