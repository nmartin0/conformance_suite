/*
 * When math_errhandling includes MATH_ERREXCEPT, <fenv.h> defines
 *   FE_DIVBYZERO, FE_INVALID and FE_OVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.005
 */
#include <math.h>
#include <fenv.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.math.h.005");
	pc_test();
	if ((math_errhandling & MATH_ERREXCEPT) == 0)
		pc_notinuse("math_errhandling does not include MATH_ERREXCEPT");
#if !defined(FE_DIVBYZERO)
	pc_fail("FE_DIVBYZERO is not defined although math_errhandling includes MATH_ERREXCEPT");
#elif !defined(FE_INVALID)
	pc_fail("FE_INVALID is not defined although math_errhandling includes MATH_ERREXCEPT");
#elif !defined(FE_OVERFLOW)
	pc_fail("FE_OVERFLOW is not defined although math_errhandling includes MATH_ERREXCEPT");
#else
	pc_pass();
#endif
}
