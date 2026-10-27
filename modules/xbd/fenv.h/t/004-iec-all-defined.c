/*
 * With IEC 60559 floating point, all five exception macros and all four
 *   rounding macros are defined.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.004
 */
#include <fenv.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.fenv.h.004");
	pc_test();
#if !defined(__STDC_IEC_559__) && !defined(__STDC_IEC_60559_BFP__)
	pc_notinuse("the implementation does not claim IEC 60559 floating point");
#else
#ifndef FE_DIVBYZERO
	pc_fail("FE_DIVBYZERO is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_INEXACT
	pc_fail("FE_INEXACT is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_INVALID
	pc_fail("FE_INVALID is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_OVERFLOW
	pc_fail("FE_OVERFLOW is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_UNDERFLOW
	pc_fail("FE_UNDERFLOW is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_DOWNWARD
	pc_fail("FE_DOWNWARD is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_TONEAREST
	pc_fail("FE_TONEAREST is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_TOWARDZERO
	pc_fail("FE_TOWARDZERO is not defined although IEC 60559 floating point is claimed");
#endif
#ifndef FE_UPWARD
	pc_fail("FE_UPWARD is not defined although IEC 60559 floating point is claimed");
#endif
	pc_pass();
#endif
}
