/*
 * round() leaves a large integral value alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.round.002
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	double big = 1;
	int i;

	for (i = 0; i < DBL_MANT_DIG; i++)
		big *= 2;	/* beyond this magnitude every value is integral */
	x = big;
	r = round(x);
	pc_check(r == big, "round(%g) returned %g, expected the argument", (double)x, (double)r);
	x = -big;
	r = round(x);
	pc_check(r == -big, "round(%g) returned %g, expected the argument", (double)x, (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	float big = 1;
	int i;

	for (i = 0; i < FLT_MANT_DIG; i++)
		big *= 2;	/* beyond this magnitude every value is integral */
	x = big;
	r = roundf(x);
	pc_check(r == big, "roundf(%g) returned %g, expected the argument", (double)x, (double)r);
	x = -big;
	r = roundf(x);
	pc_check(r == -big, "roundf(%g) returned %g, expected the argument", (double)x, (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	long double big = 1;
	int i;

	for (i = 0; i < LDBL_MANT_DIG; i++)
		big *= 2;	/* beyond this magnitude every value is integral */
	x = big;
	r = roundl(x);
	pc_check(r == big, "roundl(%g) returned %g, expected the argument", (double)x, (double)r);
	x = -big;
	r = roundl(x);
	pc_check(r == -big, "roundl(%g) returned %g, expected the argument", (double)x, (double)r);
}

int main(void)
{
	pc_begin("XSH.round.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
