/*
 * NAN, when defined, is a constant expression of type float that is a
 *   NaN; it is defined where IEC 60559 is claimed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.009
 */
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#ifdef NAN
static const float nanf_value = NAN;
#endif

int main(void)
{
	pc_begin("XBD.math.h.009");
	pc_test();
#if !defined(NAN)
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
	pc_fail("<math.h> does not define NAN although IEC 60559 is claimed");
#else
	pc_notinuse("NAN is not defined (quiet NaNs not supported)");
#endif
#else
	pc_check(IS_T(NAN, float), "NAN does not have type float");
	pc_check(isnan(nanf_value), "NAN is not a NaN");
	pc_pass();
#endif
}
