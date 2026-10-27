/*
 * CMPLX components.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.CMPLX.001
 */
#include "pc.h"
#include <complex.h>
#include <math.h>

int main(void)
{
	static const double xs[] = { 0.0, 1.0, -2.5, 3.25e10, -1e-300 };
	size_t i;

	pc_begin("XSH.CMPLX.001");
	pc_test();
	for (i = 0; i < sizeof xs / sizeof xs[0]; i++) {
		double x = xs[i], y = xs[(i + 2) % 5];
		double complex z = CMPLX(x, y);
		float complex zf = CMPLXF((float)x, (float)y);
		long double complex zl = CMPLXL((long double)x, (long double)y);

		pc_check(creal(z) == x && cimag(z) == y, "CMPLX(%g, %g) has parts %g and %g", x, y, creal(z), cimag(z));
		pc_check(crealf(zf) == (float)x && cimagf(zf) == (float)y, "CMPLXF(%g, %g) has the wrong parts", x, y);
		pc_check(creall(zl) == (long double)x && cimagl(zl) == (long double)y, "CMPLXL(%g, %g) has the wrong parts", x, y);
	}
	{
		double complex z = CMPLX(7, -9);	/* integer arguments */

		pc_check(creal(z) == 7.0 && cimag(z) == -9.0, "CMPLX(7, -9) has parts %g and %g", creal(z), cimag(z));
	}
	pc_pass();
}
