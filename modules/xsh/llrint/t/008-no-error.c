/*
 * llrint() of an ordinary value reports no error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llrint.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x = 2.5;
	volatile long long r;

	MT_START_E();
	r = llrint(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llrint");
}

static void run_f(void)
{
	volatile float x = 2.5f;
	volatile long long r;

	MT_START_E();
	r = llrintf(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llrintf");
}

static void run_l(void)
{
	volatile long double x = 2.5L;
	volatile long long r;

	MT_START_E();
	r = llrintl(x);
	MT_SAVE();
	(void)r;
	mt_noerr("llrintl");
}

int main(void)
{
	pc_begin("XSH.llrint.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
