/*
 * The classification and comparison macros yield int for float, double
 *   and long double arguments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.002
 */
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#define ONE(m) pc_check(IS_T(m(f), int) && IS_T(m(d), int) && IS_T(m(l), int), \
    #m "() does not yield an int for every real-floating type")
#define TWO(m) pc_check(IS_T(m(f, f), int) && IS_T(m(d, d), int) && \
    IS_T(m(l, l), int) && IS_T(m(f, l), int), \
    #m "() does not yield an int for every real-floating type")

int main(void)
{
	float f = 1.0f;
	double d = 1.0;
	long double l = 1.0L;

	pc_begin("XBD.math.h.002");
	pc_test();
	ONE(fpclassify);
	ONE(isfinite);
	ONE(isinf);
	ONE(isnan);
	ONE(isnormal);
	ONE(signbit);
	TWO(isgreater);
	TWO(isgreaterequal);
	TWO(isless);
	TWO(islessequal);
	TWO(islessgreater);
	TWO(isunordered);
	pc_pass();
}
