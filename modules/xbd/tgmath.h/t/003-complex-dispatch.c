/*
 * A type-generic macro called with a complex argument picks the complex
 *   function of the right type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tgmath.h.003
 */
#include <tgmath.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)
#define T(expr, type) pc_check(IS_T(expr, type), #expr " does not have type " #type)

int main(void)
{
	float f = 1.5f;
	double d = 1.5;
	long double ld = 1.5L;
	int n = 1;
	float complex fc = 1.0f;
	double complex dc = 1.0;
	long double complex ldc = 1.0L;

	pc_begin("XBD.tgmath.h.003");
	pc_test();
	T(exp(fc), float complex);
	T(exp(dc), double complex);
	T(exp(ldc), long double complex);
	T(log(fc), float complex);
	T(sqrt(dc), double complex);
	T(sqrt(ldc), long double complex);
	T(sin(fc), float complex);
	T(cos(dc), double complex);
	T(tan(ldc), long double complex);
	T(asin(fc), float complex);
	T(acos(dc), double complex);
	T(atan(ldc), long double complex);
	T(sinh(fc), float complex);
	T(cosh(dc), double complex);
	T(tanh(ldc), long double complex);
	T(asinh(fc), float complex);
	T(acosh(dc), double complex);
	T(atanh(ldc), long double complex);
	T(pow(ldc, f), long double complex);
	T(pow(f, dc), double complex);
	T(pow(fc, fc), float complex);
	T(pow(fc, ld), long double complex);
	T(pow(n, fc), double complex);
	T(fabs(fc), float);
	T(fabs(dc), double);
	T(fabs(ldc), long double);
	T(carg(fc), float);
	T(carg(dc), double);
	T(carg(ldc), long double);
	T(carg(n), double);
	T(carg(f), float);
	T(carg(ld), long double);
	T(cimag(fc), float);
	T(cimag(ld), long double);
	T(cimag(n), double);
	T(cimag(dc), double);
	T(creal(d), double);
	T(creal(ldc), long double);
	T(creal(f), float);
	T(creal(fc), float);
	T(conj(f), float complex);
	T(conj(n), double complex);
	T(conj(ld), long double complex);
	T(conj(dc), double complex);
	T(cproj(f), float complex);
	T(cproj(ldc), long double complex);
	T(cproj(d), double complex);
	T(cproj(fc), float complex);
	(void)f; (void)d; (void)ld; (void)n; (void)fc; (void)dc; (void)ldc;
	pc_pass();
}
