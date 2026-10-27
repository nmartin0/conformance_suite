/*
 * math_errhandling has type int, a value of 1, 2 or 3, and does not
 *   change.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.004
 */
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	volatile double x = 2.0;
	volatile double r;
	int before, after;

	pc_begin("XBD.math.h.004");
	pc_test();
#if !defined(MATH_ERRNO) || !defined(MATH_ERREXCEPT)
	pc_fail("<math.h> does not define MATH_ERRNO and MATH_ERREXCEPT");
#else
	before = math_errhandling;
	pc_check(IS_T(math_errhandling, int), "math_errhandling does not have type int");
	r = sqrt(x);
	(void)r;
	after = math_errhandling;
	pc_check(before == after, "math_errhandling changed from %d to %d", before, after);
	pc_check(before == MATH_ERRNO || before == MATH_ERREXCEPT ||
	    before == (MATH_ERRNO | MATH_ERREXCEPT),
	    "math_errhandling is %d, not MATH_ERRNO, MATH_ERREXCEPT or both", before);
	pc_pass();
#endif
}
