/*
 * CMPLX, CMPLXF and CMPLXL build a complex value of the right type from
 *   two real parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.007
 */
#include <complex.h>
#include <math.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static const double complex sd = CMPLX(1.5, -2.5);
static const float complex sf = CMPLXF(1.5f, -2.5f);
static const long double complex sl = CMPLXL(1.5L, -2.5L);

int main(void)
{
	pc_begin("XBD.complex.h.007");
	pc_test();
	pc_check(IS_T(CMPLX(1, 2), double complex), "CMPLX does not give a double complex");
	pc_check(IS_T(CMPLXF(1, 2), float complex), "CMPLXF does not give a float complex");
	pc_check(IS_T(CMPLXL(1, 2), long double complex), "CMPLXL does not give a long double complex");
	pc_check(creal(sd) == 1.5 && cimag(sd) == -2.5, "CMPLX(1.5, -2.5) is (%g, %g)", creal(sd), cimag(sd));
	pc_check(crealf(sf) == 1.5f && cimagf(sf) == -2.5f, "CMPLXF(1.5, -2.5) is (%g, %g)", (double)crealf(sf), (double)cimagf(sf));
	pc_check(creall(sl) == 1.5L && cimagl(sl) == -2.5L, "CMPLXL(1.5, -2.5) is (%Lg, %Lg)", creall(sl), cimagl(sl));
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
	{
		double complex a = CMPLX(-0.0, 0.0), b = CMPLX(0.0, -0.0), c = CMPLX(1.0, INFINITY), e = CMPLX(INFINITY, 1.0);
		float complex af = CMPLXF(-0.0f, 0.0f), cf = CMPLXF(1.0f, INFINITY);
		long double complex al = CMPLXL(-0.0L, 0.0L), cl = CMPLXL(1.0L, INFINITY);

		pc_check(signbit(creal(a)) && !signbit(cimag(a)), "CMPLX(-0.0, 0.0) lost the sign of a zero part");
		pc_check(!signbit(creal(b)) && signbit(cimag(b)), "CMPLX(0.0, -0.0) lost the sign of a zero part");
		pc_check(creal(c) == 1.0 && isinf(cimag(c)), "CMPLX(1, inf) did not keep both parts");
		pc_check(isinf(creal(e)) && cimag(e) == 1.0, "CMPLX(inf, 1) did not keep both parts");
		pc_check(signbit(crealf(af)) && !signbit(cimagf(af)), "CMPLXF(-0.0, 0.0) lost the sign of a zero part");
		pc_check(crealf(cf) == 1.0f && isinf(cimagf(cf)), "CMPLXF(1, inf) did not keep both parts");
		pc_check(signbit(creall(al)) && !signbit(cimagl(al)), "CMPLXL(-0.0, 0.0) lost the sign of a zero part");
		pc_check(creall(cl) == 1.0L && isinf(cimagl(cl)), "CMPLXL(1, inf) did not keep both parts");
	}
#endif
	pc_pass();
}
