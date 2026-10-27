/*
 * HUGE_VAL, HUGE_VALF and HUGE_VALL are positive constant expressions of
 *   type double, float and long double.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.006
 */
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static const double hv = HUGE_VAL;
static const float hvf = HUGE_VALF;
static const long double hvl = HUGE_VALL;

int main(void)
{
	pc_begin("XBD.math.h.006");
	pc_test();
	pc_check(IS_T(HUGE_VAL, double), "HUGE_VAL does not have type double");
	pc_check(IS_T(HUGE_VALF, float), "HUGE_VALF does not have type float");
	pc_check(IS_T(HUGE_VALL, long double), "HUGE_VALL does not have type long double");
	pc_check(hv > 0, "HUGE_VAL is not positive");
	pc_check(hvf > 0, "HUGE_VALF is not positive");
	pc_check(hvl > 0, "HUGE_VALL is not positive");
	pc_pass();
}
