/*
 * imaginary and _Imaginary_I are defined together; I is _Imaginary_I or
 *   _Complex_I accordingly.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.004
 */
#include <complex.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.complex.h.004");
	pc_test();
#if defined(imaginary) && !defined(_Imaginary_I)
	pc_fail("imaginary is defined but _Imaginary_I is not");
#elif !defined(imaginary) && defined(_Imaginary_I)
	pc_fail("_Imaginary_I is defined but imaginary is not");
#elif defined(_Imaginary_I)
	pc_check(IS_T(_Imaginary_I, float _Imaginary), "_Imaginary_I does not have type float _Imaginary");
	pc_check(IS_T(I, float _Imaginary) || IS_T(I, float _Complex), "I has neither type float _Imaginary nor float _Complex");
	pc_pass();
#else
	pc_check(IS_T(I, float _Complex), "I does not have type float _Complex");
	pc_check(crealf(I) == crealf(_Complex_I) && cimagf(I) == cimagf(_Complex_I), "I is not _Complex_I");
	pc_pass();
#endif
}
