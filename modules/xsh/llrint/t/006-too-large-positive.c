/*
 * llrint() of a too large positive value is a domain error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llrint.006
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x;
	volatile long long r;
	double big = 1;
	int i;

	for (i = 0; i < (int)(sizeof(long long) * CHAR_BIT); i++)
		big *= 2;	/* 2^width: beyond every value of the integer type */
	if (isinf(big))
		return;
	x = big;
	MT_START();
	r = llrint(x);
	MT_SAVE();
	(void)r;
	mt_domain("llrint");
}

static void run_f(void)
{
	volatile float x;
	volatile long long r;
	float big = 1;
	int i;

	for (i = 0; i < (int)(sizeof(long long) * CHAR_BIT); i++)
		big *= 2;	/* 2^width: beyond every value of the integer type */
	if (isinf(big))
		return;
	x = big;
	MT_START();
	r = llrintf(x);
	MT_SAVE();
	(void)r;
	mt_domain("llrintf");
}

static void run_l(void)
{
	volatile long double x;
	volatile long long r;
	long double big = 1;
	int i;

	for (i = 0; i < (int)(sizeof(long long) * CHAR_BIT); i++)
		big *= 2;	/* 2^width: beyond every value of the integer type */
	if (isinf(big))
		return;
	x = big;
	MT_START();
	r = llrintl(x);
	MT_SAVE();
	(void)r;
	mt_domain("llrintl");
}

int main(void)
{
	pc_begin("XSH.llrint.006");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
