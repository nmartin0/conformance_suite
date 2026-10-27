/*
 * FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL and FP_ZERO are integer
 *   constant expressions with distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.001
 */
#include <math.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.math.h.001");
	pc_test();
#if !defined(FP_INFINITE) || !defined(FP_NAN) || !defined(FP_NORMAL) || \
    !defined(FP_SUBNORMAL) || !defined(FP_ZERO)
	pc_fail("<math.h> does not define all five FP_ classification macros");
#elif FP_INFINITE == FP_NAN || FP_INFINITE == FP_NORMAL || \
    FP_INFINITE == FP_SUBNORMAL || FP_INFINITE == FP_ZERO || \
    FP_NAN == FP_NORMAL || FP_NAN == FP_SUBNORMAL || FP_NAN == FP_ZERO || \
    FP_NORMAL == FP_SUBNORMAL || FP_NORMAL == FP_ZERO || \
    FP_SUBNORMAL == FP_ZERO
	pc_fail("two of the FP_ classification macros have the same value");
#else
	pc_pass();
#endif
}
