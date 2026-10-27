/*
 * CMPLX result types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.CMPLX.003
 */
#include "pc.h"
#include <complex.h>

#define KIND(e) _Generic((e), double complex: 1, float complex: 2, long double complex: 3, default: 0)

int main(void)
{
	pc_begin("XSH.CMPLX.003");
	pc_test();
	pc_check(KIND(CMPLX(1.0, 2.0)) == 1, "CMPLX(double, double) is not a double complex");
	pc_check(KIND(CMPLX(1.0f, 2.0f)) == 1, "CMPLX(float, float) is not a double complex");
	pc_check(KIND(CMPLX(1, 2)) == 1, "CMPLX(int, int) is not a double complex");
	pc_check(KIND(CMPLXF(1.0, 2.0)) == 2, "CMPLXF(double, double) is not a float complex");
	pc_check(KIND(CMPLXF(1, 2)) == 2, "CMPLXF(int, int) is not a float complex");
	pc_check(KIND(CMPLXL(1.0, 2.0)) == 3, "CMPLXL(double, double) is not a long double complex");
	pc_check(KIND(CMPLXL(1, 2)) == 3, "CMPLXL(int, int) is not a long double complex");
	pc_pass();
}
