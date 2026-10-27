/*
 * isinf() judges the semantic type of its argument.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isinf.003
 */
#include "mt.h"

static void run_d(void)
{
	volatile double a = DBL_MAX;
	volatile int r;

	if (sizeof(double) == sizeof(long double))
		return;		/* nothing wider to evaluate in */
	r = isinf(a * 2);
	pc_check(r != 0, "isinf(MAX * 2) gave %d for double: the product was not converted to the semantic type", r);
}

static void run_f(void)
{
	volatile float a = FLT_MAX;
	volatile int r;

	if (sizeof(float) == sizeof(long double))
		return;		/* nothing wider to evaluate in */
	r = isinf(a * 2);
	pc_check(r != 0, "isinf(MAX * 2) gave %d for float: the product was not converted to the semantic type", r);
}

static void run_l(void)
{
	volatile long double a = LDBL_MAX;
	volatile int r;

	if (sizeof(long double) == sizeof(long double))
		return;		/* nothing wider to evaluate in */
	r = isinf(a * 2);
	pc_check(r != 0, "isinf(MAX * 2) gave %d for long double: the product was not converted to the semantic type", r);
}

int main(void)
{
	pc_begin("XSH.isinf.003");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
