/*
 * I*I is -1 as a complex number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.005
 */
#include <complex.h>
#include "pc.h"


int main(void)
{
	float complex f = I * I;
	double complex d = I * I;
	long double complex l = I * I;

	pc_begin("XBD.complex.h.005");
	pc_test();
	pc_check(crealf(f) == -1.0f && cimagf(f) == 0.0f, "I*I is (%g, %g) as float complex", (double)crealf(f), (double)cimagf(f));
	pc_check(creal(d) == -1.0 && cimag(d) == 0.0, "I*I is (%g, %g) as double complex", creal(d), cimag(d));
	pc_check(creall(l) == -1.0L && cimagl(l) == 0.0L, "I*I is (%Lg, %Lg) as long double complex", creall(l), cimagl(l));
	pc_pass();
}
