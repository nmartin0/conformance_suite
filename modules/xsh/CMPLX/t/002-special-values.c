/*
 * CMPLX with zeros, infinities and NaNs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.CMPLX.002
 */
#include "pc.h"
#include <complex.h>
#include <math.h>

int main(void)
{
	double complex a, b, c, d, e, f;
	float complex af, cf, ef;
	long double complex al, cl, el;

	pc_begin("XSH.CMPLX.002");
	pc_test();
	a = CMPLX(-0.0, -0.0);
	pc_check(creal(a) == 0.0 && signbit(creal(a)), "CMPLX(-0.0, -0.0) lost the sign of the real part");
	pc_check(cimag(a) == 0.0 && signbit(cimag(a)), "CMPLX(-0.0, -0.0) lost the sign of the imaginary part");
	b = CMPLX(0.0, -0.0);
	pc_check(creal(b) == 0.0 && !signbit(creal(b)), "CMPLX(0.0, -0.0) real part is not +0");
	pc_check(cimag(b) == 0.0 && signbit(cimag(b)), "CMPLX(0.0, -0.0) imaginary part is not -0");
	c = CMPLX(-0.0, 0.0);
	pc_check(signbit(creal(c)) && !signbit(cimag(c)), "CMPLX(-0.0, 0.0) has the wrong signs");
	d = CMPLX(INFINITY, 1.0);
	pc_check(isinf(creal(d)) && creal(d) > 0 && cimag(d) == 1.0, "CMPLX(INFINITY, 1.0) has parts %g and %g", creal(d), cimag(d));
	e = CMPLX(1.0, -INFINITY);
	pc_check(creal(e) == 1.0 && isinf(cimag(e)) && cimag(e) < 0, "CMPLX(1.0, -INFINITY) has parts %g and %g", creal(e), cimag(e));
	f = CMPLX(NAN, 2.0);
	pc_check(isnan(creal(f)) && cimag(f) == 2.0, "CMPLX(NAN, 2.0) has parts %g and %g", creal(f), cimag(f));
	f = CMPLX(3.0, NAN);
	pc_check(creal(f) == 3.0 && isnan(cimag(f)), "CMPLX(3.0, NAN) has parts %g and %g", creal(f), cimag(f));

	af = CMPLXF(-0.0f, -0.0f);
	pc_check(signbit(crealf(af)) && signbit(cimagf(af)), "CMPLXF(-0.0, -0.0) lost a sign");
	cf = CMPLXF(INFINITY, 1.0f);
	pc_check(isinf(crealf(cf)) && cimagf(cf) == 1.0f, "CMPLXF(INFINITY, 1) is wrong");
	ef = CMPLXF(NAN, 2.0f);
	pc_check(isnan(crealf(ef)) && cimagf(ef) == 2.0f, "CMPLXF(NAN, 2) is wrong");

	al = CMPLXL(-0.0L, -0.0L);
	pc_check(signbit(creall(al)) && signbit(cimagl(al)), "CMPLXL(-0.0, -0.0) lost a sign");
	cl = CMPLXL(INFINITY, 1.0L);
	pc_check(isinf(creall(cl)) && cimagl(cl) == 1.0L, "CMPLXL(INFINITY, 1) is wrong");
	el = CMPLXL(NAN, 2.0L);
	pc_check(isnan(creall(el)) && cimagl(el) == 2.0L, "CMPLXL(NAN, 2) is wrong");
	pc_pass();
}
