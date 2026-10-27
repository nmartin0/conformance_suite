/*
 * INFINITY is a positive constant expression of type float, and
 *   +infinity where IEC 60559 is claimed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.008
 */
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#ifdef INFINITY
static const float inf = INFINITY;
#endif

int main(void)
{
	pc_begin("XBD.math.h.008");
	pc_test();
#if !defined(INFINITY)
	pc_fail("<math.h> does not define INFINITY");
#else
	pc_check(IS_T(INFINITY, float), "INFINITY does not have type float");
	pc_check(inf > 0, "INFINITY is not positive");
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
	pc_check(isinf(inf), "INFINITY is not infinite on an IEC 60559 implementation");
#endif
	pc_pass();
#endif
}
