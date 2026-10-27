/*
 * With imaginary types, the macros give a real, imaginary or complex
 *   result as the page lists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tgmath.h.005
 */
#include <tgmath.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)
#define T(expr, type) pc_check(IS_T(expr, type), #expr " does not have type " #type)

#if defined(_Imaginary_I) && (defined(__STDC_IEC_559_COMPLEX__) || defined(__STDC_IEC_60559_COMPLEX__))
int main(void)
{
	float _Imaginary im = _Imaginary_I;
	double _Imaginary iy = 2.0 * _Imaginary_I;
	double complex s;
	double c;

	pc_begin("XBD.tgmath.h.005");
	pc_test();
	T(cos(im), float);
	T(cosh(im), float);
	T(fabs(im), float);
	T(carg(im), float);
	T(cimag(im), float);
	T(creal(im), float);
	T(sin(im), float _Imaginary);
	T(tan(im), float _Imaginary);
	T(sinh(im), float _Imaginary);
	T(tanh(im), float _Imaginary);
	T(asin(im), float _Imaginary);
	T(atan(im), float _Imaginary);
	T(asinh(im), float _Imaginary);
	T(atanh(im), float _Imaginary);
	T(exp(im), float complex);
	T(log(im), float complex);
	T(sqrt(im), float complex);
	T(acos(im), float complex);
	T(acosh(im), float complex);
	T(conj(im), float complex);
	T(cproj(im), float complex);
	c = cos(iy);
	pc_check(fabs(c - cosh(2.0)) < 1e-12, "cos(2i) is %g, not cosh(2)", c);
	s = sin(iy);
	pc_check(creal(s) == 0.0 && fabs(cimag(s) - sinh(2.0)) < 1e-12, "sin(2i) is (%g, %g), not i sinh(2)", creal(s), cimag(s));
	(void)im;
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XBD.tgmath.h.005");
	pc_test();
	pc_notinuse("the implementation does not claim the IEC 60559 complex option or has no imaginary types");
}
#endif
