/*
 * HUGE_VAL, HUGE_VALF and HUGE_VALL are positive; INFINITY and NAN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.008
 */
#include "mt.h"

int main(void)
{
	pc_begin("XSH.fpclassify.008");
	pc_test();
	volatile double hd = HUGE_VAL;
	volatile float hf = HUGE_VALF;
	volatile long double hl = HUGE_VALL;

	pc_check(hd > 0, "HUGE_VAL is not positive");
	pc_check(hf > 0, "HUGE_VALF is not positive");
	pc_check(hl > 0, "HUGE_VALL is not positive");
	if (MT_MX) {
		volatile float inf = INFINITY;

		pc_check(isinf(hd) && isinf(hf) && isinf(hl), "HUGE_VAL, HUGE_VALF and HUGE_VALL are not all infinite");
		pc_check(isinf(inf) && inf > 0, "INFINITY is not positive infinity");
	}
	#if !MT_NO_NAN
	{
		volatile float n = NAN;

		pc_check(isnan(n), "NAN is not a NaN");
	}
	#endif
	pc_pass();
}
