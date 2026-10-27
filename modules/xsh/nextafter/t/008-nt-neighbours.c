/*
 * nexttoward() returns the adjacent representable value toward y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.008
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	volatile long double y;

	x = 1;
	y = 2.0L;
	r = nexttoward(x, y);
	pc_check(r == 1 + DBL_EPSILON, "nexttoward(1, 2) returned %.20g", (double)r);
	y = 0.0L;
	r = nexttoward(x, y);
	pc_check(r == 1 - DBL_EPSILON / FLT_RADIX, "nexttoward(1, 0) returned %.20g", (double)r);
	y = 1.0L + LDBL_EPSILON;
	r = nexttoward(x, y);
	pc_check(r == 1 + DBL_EPSILON, "nexttoward(1, 1 + LDBL_EPSILON) returned %.20g", (double)r);
}

static void run_f(void)
{
	volatile float x, r;
	volatile long double y;

	x = 1;
	y = 2.0L;
	r = nexttowardf(x, y);
	pc_check(r == 1 + FLT_EPSILON, "nexttowardf(1, 2) returned %.20g", (double)r);
	y = 0.0L;
	r = nexttowardf(x, y);
	pc_check(r == 1 - FLT_EPSILON / FLT_RADIX, "nexttowardf(1, 0) returned %.20g", (double)r);
	y = 1.0L + LDBL_EPSILON;
	r = nexttowardf(x, y);
	pc_check(r == 1 + FLT_EPSILON, "nexttowardf(1, 1 + LDBL_EPSILON) returned %.20g", (double)r);
}

static void run_l(void)
{
	volatile long double x, r;
	volatile long double y;

	x = 1;
	y = 2.0L;
	r = nexttowardl(x, y);
	pc_check(r == 1 + LDBL_EPSILON, "nexttowardl(1, 2) returned %.20g", (double)r);
	y = 0.0L;
	r = nexttowardl(x, y);
	pc_check(r == 1 - LDBL_EPSILON / FLT_RADIX, "nexttowardl(1, 0) returned %.20g", (double)r);
	y = 1.0L + LDBL_EPSILON;
	r = nexttowardl(x, y);
	pc_check(r == 1 + LDBL_EPSILON, "nexttowardl(1, 1 + LDBL_EPSILON) returned %.20g", (double)r);
}

int main(void)
{
	pc_begin("XSH.nextafter.008");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
