/*
 * The result of nextafter() does not depend on the rounding direction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.007
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, r;
	int m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		x = 1;
		y = 2;
		r = nextafter(x, y);
		pc_check(r == 1 + DBL_EPSILON, "nextafter(1, 2) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
		y = 0;
		r = nextafter(x, y);
		pc_check(r == 1 - DBL_EPSILON / FLT_RADIX, "nextafter(1, 0) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
	}
	mt_leave();
}

static void run_f(void)
{
	volatile float x, y, r;
	int m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		x = 1;
		y = 2;
		r = nextafterf(x, y);
		pc_check(r == 1 + FLT_EPSILON, "nextafterf(1, 2) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
		y = 0;
		r = nextafterf(x, y);
		pc_check(r == 1 - FLT_EPSILON / FLT_RADIX, "nextafterf(1, 0) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
	}
	mt_leave();
}

static void run_l(void)
{
	volatile long double x, y, r;
	int m;

	for (m = 0; m < MT_NMODES; m++) {
		if (!mt_enter(m))
			continue;
		x = 1;
		y = 2;
		r = nextafterl(x, y);
		pc_check(r == 1 + LDBL_EPSILON, "nextafterl(1, 2) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
		y = 0;
		r = nextafterl(x, y);
		pc_check(r == 1 - LDBL_EPSILON / FLT_RADIX, "nextafterl(1, 0) returned %.20g (rounding mode %d)", (double)r, mt_modes[m]);
	}
	mt_leave();
}

int main(void)
{
	pc_begin("XSH.nextafter.007");
	MT_NEED_MX();
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_need_modes(2);
	pc_pass();
}
