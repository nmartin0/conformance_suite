/*
 * The macros call the function their argument types call for: complex
 *   for complex, real for real.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tgmath.h.004
 */
#include <tgmath.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)


int main(void)
{
	volatile double neg = -1.0;
	double complex z = CMPLX(-4.0, 0.0), r;
	double complex w = CMPLX(3.0, 4.0);
	double p;
	int a = 2, b = 3;

	pc_begin("XBD.tgmath.h.004");
	pc_test();
	/* the page fixes which function runs, not the accuracy of its result */
	r = sqrt(z);
	pc_check(fabs(creal(r)) < 1e-9 && fabs(cimag(r) - 2.0) < 1e-9, "sqrt(-4+0i) is (%g, %g), not (0, 2)", creal(r), cimag(r));
	pc_check(fabs(fabs(w) - 5.0) < 1e-9, "fabs(3+4i) is %g, not 5", (double)fabs(w));
	/* the real function, not the complex root: a real result (a NaN is only
	   required where the system supports one) */
	pc_check(IS_T(sqrt(neg), double), "sqrt of the real -1 does not have type double");
#ifdef NAN
	pc_check(isnan(sqrt(neg)), "sqrt of the real -1 is not a NaN");
#endif
	p = pow(a, b);
	pc_check(fabs(p - 8.0) < 1e-9, "pow(2, 3) is %g, not 8", p);
	pc_check(fabs(sqrt(4.0f) - 2.0f) < 1e-3f, "sqrt(4.0f) is not 2");
	pc_check(creal(3.0) == 3.0 && cimag(3.0) == 0.0, "creal or cimag of a real argument is wrong");
	r = conj(w);
	pc_check(creal(r) == 3.0 && cimag(r) == -4.0, "conj(3+4i) is (%g, %g)", creal(r), cimag(r));
	r = exp(CMPLX(0.0, 0.0));
	pc_check(fabs(creal(r) - 1.0) < 1e-9 && fabs(cimag(r)) < 1e-9, "exp(0+0i) is (%g, %g), not (1, 0)", creal(r), cimag(r));
	pc_pass();
}
