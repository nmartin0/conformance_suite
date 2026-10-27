/*
 * nexttoward() returns y converted to the function type when x equals y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.009
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 3.0, -2.5, 0.1 };
	volatile double x, r;
	volatile long double y;
	int i;

	for (i = 0; i < 3; i++) {
		x = in[i];
		y = in[i];
		r = nexttoward(x, y);
		pc_check(r == in[i], "nexttoward(%g, same) returned %g, expected the argument", (double)x, (double)r);
	}
	if (MT_MX) {
		x = -0.0;
		y = 0.0L;
		r = nexttoward(x, y);
		pc_check(MT_SAME(r, (double)0.0), "nexttoward(-0, +0) returned %g, expected +0", (double)r);
		x = 0.0;
		y = -0.0L;
		r = nexttoward(x, y);
		pc_check(MT_SAME(r, (double)-0.0), "nexttoward(+0, -0) returned %g, expected -0", (double)r);
	}
}

static void run_f(void)
{
	static const float in[] = { 3.0f, -2.5f, 0.1f };
	volatile float x, r;
	volatile long double y;
	int i;

	for (i = 0; i < 3; i++) {
		x = in[i];
		y = in[i];
		r = nexttowardf(x, y);
		pc_check(r == in[i], "nexttowardf(%g, same) returned %g, expected the argument", (double)x, (double)r);
	}
	if (MT_MX) {
		x = -0.0f;
		y = 0.0L;
		r = nexttowardf(x, y);
		pc_check(MT_SAME(r, (float)0.0f), "nexttowardf(-0, +0) returned %g, expected +0", (double)r);
		x = 0.0f;
		y = -0.0L;
		r = nexttowardf(x, y);
		pc_check(MT_SAME(r, (float)-0.0f), "nexttowardf(+0, -0) returned %g, expected -0", (double)r);
	}
}

static void run_l(void)
{
	static const long double in[] = { 3.0L, -2.5L, 0.1L };
	volatile long double x, r;
	volatile long double y;
	int i;

	for (i = 0; i < 3; i++) {
		x = in[i];
		y = in[i];
		r = nexttowardl(x, y);
		pc_check(r == in[i], "nexttowardl(%g, same) returned %g, expected the argument", (double)x, (double)r);
	}
	if (MT_MX) {
		x = -0.0L;
		y = 0.0L;
		r = nexttowardl(x, y);
		pc_check(MT_SAME(r, (long double)0.0L), "nexttowardl(-0, +0) returned %g, expected +0", (double)r);
		x = 0.0L;
		y = -0.0L;
		r = nexttowardl(x, y);
		pc_check(MT_SAME(r, (long double)-0.0L), "nexttowardl(+0, -0) returned %g, expected -0", (double)r);
	}
}

int main(void)
{
	pc_begin("XSH.nextafter.009");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
