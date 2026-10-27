/*
 * nearbyint() does not raise the inexact exception.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nearbyint.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 2.5, 0.75, -1.25 };
	volatile double x, r;
	int i, fl;

	for (i = 0; i < 3; i++) {
		x = in[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = nearbyint(x);
		fl = fetestexcept(FE_INEXACT);
		(void)r;
		pc_check(fl == 0, "nearbyint(%g) raised the inexact exception", (double)x);
	}
}

static void run_f(void)
{
	static const float in[] = { 2.5f, 0.75f, -1.25f };
	volatile float x, r;
	int i, fl;

	for (i = 0; i < 3; i++) {
		x = in[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = nearbyintf(x);
		fl = fetestexcept(FE_INEXACT);
		(void)r;
		pc_check(fl == 0, "nearbyintf(%g) raised the inexact exception", (double)x);
	}
}

static void run_l(void)
{
	static const long double in[] = { 2.5L, 0.75L, -1.25L };
	volatile long double x, r;
	int i, fl;

	for (i = 0; i < 3; i++) {
		x = in[i];
		(void)feclearexcept(FE_ALL_EXCEPT);
		r = nearbyintl(x);
		fl = fetestexcept(FE_INEXACT);
		(void)r;
		pc_check(fl == 0, "nearbyintl(%g) raised the inexact exception", (double)x);
	}
}

int main(void)
{
	pc_begin("XSH.nearbyint.003");
	if (!MT_HAVE_INEXACT)
		pc_notinuse("the inexact exception is not supported");
	if (feraiseexcept(FE_INEXACT) != 0 || (fetestexcept(FE_INEXACT) & FE_INEXACT) == 0)
		pc_unresolved("the inexact flag cannot be raised and tested");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
