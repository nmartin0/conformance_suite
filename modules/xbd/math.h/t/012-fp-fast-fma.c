/*
 * FP_FAST_FMA, FP_FAST_FMAF and FP_FAST_FMAL, when defined, expand to
 *   the integer constant 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.012
 */
#include <math.h>
#include "pc.h"


int main(void)
{
	int n = 0;

	pc_begin("XBD.math.h.012");
	pc_test();
#ifdef FP_FAST_FMA
#if FP_FAST_FMA != 1
	pc_fail("FP_FAST_FMA is defined but is not 1");
#endif
	n++;
#endif
#ifdef FP_FAST_FMAF
#if FP_FAST_FMAF != 1
	pc_fail("FP_FAST_FMAF is defined but is not 1");
#endif
	n++;
#endif
#ifdef FP_FAST_FMAL
#if FP_FAST_FMAL != 1
	pc_fail("FP_FAST_FMAL is defined but is not 1");
#endif
	n++;
#endif
	if (n == 0)
		pc_notinuse("none of the FP_FAST_FMA macros is defined");
	pc_pass();
}
