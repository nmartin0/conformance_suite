/*
 * fma() rounds once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fma.002
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, p, z, r, e2;

	x = 1 + DBL_EPSILON;
	y = 1 + DBL_EPSILON;
	p = x * y;
	z = -p;
	r = fma(x, y, z);
	pc_check(!isnan(r) && r != 0, "fma(1+eps, 1+eps, -(product)) returned %g: the product was rounded before the addition", (double)r);
#ifdef FE_TONEAREST
	if (fegetround() == FE_TONEAREST) {
		e2 = DBL_EPSILON * DBL_EPSILON;
		pc_check(r == e2, "fma(1+eps, 1+eps, -(product)) returned %g, expected %g", (double)r, (double)e2);
	}
#endif
	(void)e2;
}

static void run_f(void)
{
	volatile float x, y, p, z, r, e2;

	x = 1 + FLT_EPSILON;
	y = 1 + FLT_EPSILON;
	p = x * y;
	z = -p;
	r = fmaf(x, y, z);
	pc_check(!isnan(r) && r != 0, "fmaf(1+eps, 1+eps, -(product)) returned %g: the product was rounded before the addition", (double)r);
#ifdef FE_TONEAREST
	if (fegetround() == FE_TONEAREST) {
		e2 = FLT_EPSILON * FLT_EPSILON;
		pc_check(r == e2, "fmaf(1+eps, 1+eps, -(product)) returned %g, expected %g", (double)r, (double)e2);
	}
#endif
	(void)e2;
}

static void run_l(void)
{
	volatile long double x, y, p, z, r, e2;

	x = 1 + LDBL_EPSILON;
	y = 1 + LDBL_EPSILON;
	p = x * y;
	z = -p;
	r = fmal(x, y, z);
	pc_check(!isnan(r) && r != 0, "fmal(1+eps, 1+eps, -(product)) returned %g: the product was rounded before the addition", (double)r);
#ifdef FE_TONEAREST
	if (fegetround() == FE_TONEAREST) {
		e2 = LDBL_EPSILON * LDBL_EPSILON;
		pc_check(r == e2, "fmal(1+eps, 1+eps, -(product)) returned %g, expected %g", (double)r, (double)e2);
	}
#endif
	(void)e2;
}

int main(void)
{
	pc_begin("XSH.fma.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
