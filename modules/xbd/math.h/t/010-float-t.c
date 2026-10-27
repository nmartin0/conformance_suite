/*
 * float_t and double_t are real-floating types at least as wide as float
 *   and double, following FLT_EVAL_METHOD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.010
 */
#include <math.h>
#include <float.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Significand digits of a real-floating type. */
#define MANT(x) _Generic((x), float: FLT_MANT_DIG, double: DBL_MANT_DIG, \
    long double: LDBL_MANT_DIG, default: 0)

int main(void)
{
	float_t ft = 1;
	double_t dt = 1;

	pc_begin("XBD.math.h.010");
	pc_test();
	pc_check(MANT(ft) != 0, "float_t is not float, double or long double");
	pc_check(MANT(dt) != 0, "double_t is not float, double or long double");
	pc_check(MANT(ft) >= FLT_MANT_DIG, "float_t is narrower than float");
	pc_check(MANT(dt) >= DBL_MANT_DIG, "double_t is narrower than double");
	pc_check(MANT(dt) >= MANT(ft), "double_t is narrower than float_t");
#if FLT_EVAL_METHOD == 0
	pc_check(IS_T(ft, float) && IS_T(dt, double), "FLT_EVAL_METHOD is 0 but float_t, double_t are not float, double");
#elif FLT_EVAL_METHOD == 1
	pc_check(IS_T(ft, double) && IS_T(dt, double), "FLT_EVAL_METHOD is 1 but float_t, double_t are not both double");
#elif FLT_EVAL_METHOD == 2
	pc_check(IS_T(ft, long double) && IS_T(dt, long double), "FLT_EVAL_METHOD is 2 but float_t, double_t are not both long double");
#endif
	pc_pass();
}
