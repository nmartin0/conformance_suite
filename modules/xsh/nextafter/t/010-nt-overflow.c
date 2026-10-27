/*
 * nexttoward() of the largest value toward infinity is an overflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nextafter.010
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, r;
	volatile long double y;

	x = DBL_MAX;
	y = HUGE_VALL;
	MT_START();
	r = nexttoward(x, y);
	MT_SAVE();
	pc_check(r == HUGE_VAL, "nexttoward(MAX, +Inf) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("nexttoward(MAX, +Inf)");
	x = -DBL_MAX;
	y = -HUGE_VALL;
	MT_START();
	r = nexttoward(x, y);
	MT_SAVE();
	pc_check(r == -HUGE_VAL, "nexttoward(-MAX, -Inf) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("nexttoward(-MAX, -Inf)");
}

static void run_f(void)
{
	volatile float x, r;
	volatile long double y;

	x = FLT_MAX;
	y = HUGE_VALL;
	MT_START();
	r = nexttowardf(x, y);
	MT_SAVE();
	pc_check(r == HUGE_VALF, "nexttowardf(MAX, +Inf) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("nexttowardf(MAX, +Inf)");
	x = -FLT_MAX;
	y = -HUGE_VALL;
	MT_START();
	r = nexttowardf(x, y);
	MT_SAVE();
	pc_check(r == -HUGE_VALF, "nexttowardf(-MAX, -Inf) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("nexttowardf(-MAX, -Inf)");
}

static void run_l(void)
{
	volatile long double x, r;
	volatile long double y;

	x = LDBL_MAX;
	y = HUGE_VALL;
	MT_START();
	r = nexttowardl(x, y);
	MT_SAVE();
	pc_check(r == HUGE_VALL, "nexttowardl(MAX, +Inf) returned %g, expected HUGE_VAL", (double)r);
	mt_overflow("nexttowardl(MAX, +Inf)");
	x = -LDBL_MAX;
	y = -HUGE_VALL;
	MT_START();
	r = nexttowardl(x, y);
	MT_SAVE();
	pc_check(r == -HUGE_VALL, "nexttowardl(-MAX, -Inf) returned %g, expected -HUGE_VAL", (double)r);
	mt_overflow("nexttowardl(-MAX, -Inf)");
}

int main(void)
{
	pc_begin("XSH.nextafter.010");
	pc_test();
	run_d();
	run_f();
	run_l();
	mt_finish_report();
}
