/*
 * CMPLX as a static initialiser, and argument conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.CMPLX.004
 */
#include "pc.h"
#include <complex.h>

static double complex file_z = CMPLX(1.5, -2.5);
static float complex file_zf = CMPLXF(0.25, -0.5);
static long double complex file_zl = CMPLXL(3.0, 4.0);

int main(void)
{
	static double complex local_z = CMPLX(-1.0, 8.0);
	double complex w;
	float complex wf;
	long double complex wl;

	pc_begin("XSH.CMPLX.004");
	pc_test();
	pc_check(creal(file_z) == 1.5 && cimag(file_z) == -2.5, "file-scope initialiser gave the wrong value");
	pc_check(crealf(file_zf) == 0.25f && cimagf(file_zf) == -0.5f, "file-scope float initialiser gave the wrong value");
	pc_check(creall(file_zl) == 3.0L && cimagl(file_zl) == 4.0L, "file-scope long double initialiser gave the wrong value");
	pc_check(creal(local_z) == -1.0 && cimag(local_z) == 8.0, "function-local static initialiser gave the wrong value");
	wf = CMPLXF(0.1, 0.2);
	pc_check(crealf(wf) == (float)0.1 && cimagf(wf) == (float)0.2, "CMPLXF(0.1, 0.2) did not convert the arguments to float");
	wl = CMPLXL(0.1L, 0.2L);
	pc_check(creall(wl) == 0.1L && cimagl(wl) == 0.2L, "CMPLXL(0.1L, 0.2L) changed the values");
	w = CMPLX(0.1f, 0.2f);
	pc_check(creal(w) == (double)0.1f && cimag(w) == (double)0.2f, "CMPLX(0.1f, 0.2f) did not convert the float arguments to double");
	pc_pass();
}
