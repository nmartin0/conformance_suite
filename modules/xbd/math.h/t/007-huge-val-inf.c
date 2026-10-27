/*
 * Where IEEE 754 is supported, HUGE_VAL, HUGE_VALF and HUGE_VALL are
 *   +infinity.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.007
 */
#include <math.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.math.h.007");
	pc_test();
#if !defined(__STDC_IEC_559__) && !defined(__STDC_IEC_60559_BFP__)
	pc_notinuse("the implementation does not claim IEC 60559 floating point");
#else
	pc_check(isinf(HUGE_VAL) && HUGE_VAL > 0, "HUGE_VAL is not +infinity");
	pc_check(isinf(HUGE_VALF) && HUGE_VALF > 0, "HUGE_VALF is not +infinity");
	pc_check(isinf(HUGE_VALL) && HUGE_VALL > 0, "HUGE_VALL is not +infinity");
	pc_pass();
#endif
}
