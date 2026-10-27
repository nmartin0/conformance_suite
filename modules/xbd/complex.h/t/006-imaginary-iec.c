/*
 * With the IEC 60559 complex option, imaginary and _Imaginary_I are
 *   defined and I is _Imaginary_I.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.006
 */
#include <complex.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.complex.h.006");
	pc_test();
#if !defined(__STDC_IEC_559_COMPLEX__) && !defined(__STDC_IEC_60559_COMPLEX__)
	pc_notinuse("the implementation does not claim the IEC 60559 complex option");
#elif !defined(imaginary) || !defined(_Imaginary_I)
	pc_fail("imaginary or _Imaginary_I is not defined although the IEC 60559 complex option is claimed");
#else
	pc_check(IS_T(I, float _Imaginary), "I does not expand to _Imaginary_I (type float _Imaginary)");
	pc_pass();
#endif
}
