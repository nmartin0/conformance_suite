/*
 * <tgmath.h> includes <math.h> and <complex.h>, and modf() stays an
 *   ordinary function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tgmath.h.001
 */
#include <tgmath.h>
#include "pc.h"


int main(void)
{
	double ip = 0, fr;

	pc_begin("XBD.tgmath.h.001");
	pc_test();
#if !defined(FP_NAN) || !defined(HUGE_VAL) || !defined(MATH_ERRNO)
	pc_fail("<tgmath.h> does not make the <math.h> macros available");
#elif !defined(complex) || !defined(I) || !defined(_Complex_I) || !defined(CMPLX)
	pc_fail("<tgmath.h> does not make the <complex.h> macros available");
#else
	fr = modf(2.5, &ip);
	pc_check(fr == 0.5 && ip == 2.0, "modf(2.5) gave fraction %g and integral part %g", fr, ip);
	pc_pass();
#endif
}
