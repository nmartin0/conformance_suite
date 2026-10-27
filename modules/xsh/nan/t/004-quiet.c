/*
 * nan() returns a quiet NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nan.004
 */
#include "mt.h"

/* The tag goes through a volatile pointer so the call cannot be folded away. */
static const char *volatile mt_tag;

static void run_d(void)
{
	volatile double r, t;

	r = (mt_tag = "", nan(mt_tag));
	(void)feclearexcept(FE_ALL_EXCEPT);
	t = r + 1;
	(void)t;
	pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0 || FE_INVALID == 0, "arithmetic on the result of nan(\"\") raised the invalid exception: it is not quiet");
}

static void run_f(void)
{
	volatile float r, t;

	r = (mt_tag = "", nanf(mt_tag));
	(void)feclearexcept(FE_ALL_EXCEPT);
	t = r + 1;
	(void)t;
	pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0 || FE_INVALID == 0, "arithmetic on the result of nanf(\"\") raised the invalid exception: it is not quiet");
}

static void run_l(void)
{
	volatile long double r, t;

	r = (mt_tag = "", nanl(mt_tag));
	(void)feclearexcept(FE_ALL_EXCEPT);
	t = r + 1;
	(void)t;
	pc_check((fetestexcept(FE_INVALID) & FE_INVALID) == 0 || FE_INVALID == 0, "arithmetic on the result of nanl(\"\") raised the invalid exception: it is not quiet");
}

int main(void)
{
	pc_begin("XSH.nan.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
