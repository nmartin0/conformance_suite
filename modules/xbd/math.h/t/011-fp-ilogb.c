/*
 * FP_ILOGB0 is INT_MIN or -INT_MAX; FP_ILOGBNAN is INT_MAX or INT_MIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.011
 */
#include <math.h>
#include <limits.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.math.h.011");
	pc_test();
#if !defined(FP_ILOGB0) || !defined(FP_ILOGBNAN)
	pc_fail("<math.h> does not define FP_ILOGB0 and FP_ILOGBNAN");
#elif !(FP_ILOGB0 == INT_MIN || FP_ILOGB0 == -INT_MAX)
	pc_fail("FP_ILOGB0 is neither INT_MIN nor -INT_MAX");
#elif !(FP_ILOGBNAN == INT_MAX || FP_ILOGBNAN == INT_MIN)
	pc_fail("FP_ILOGBNAN is neither INT_MAX nor INT_MIN");
#else
	pc_pass();
#endif
}
