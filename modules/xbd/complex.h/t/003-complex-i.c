/*
 * _Complex_I is a constant expression of type float _Complex with the
 *   value of the imaginary unit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.complex.h.003
 */
#include <complex.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static const float _Complex unit = _Complex_I;

int main(void)
{
	float _Complex sq = _Complex_I * _Complex_I;

	pc_begin("XBD.complex.h.003");
	pc_test();
	pc_check(IS_T(_Complex_I, float _Complex), "_Complex_I does not have type float _Complex");
	pc_check(crealf(unit) == 0.0f && cimagf(unit) == 1.0f, "_Complex_I is (%g, %g), not (0, 1)", (double)crealf(unit), (double)cimagf(unit));
	pc_check(crealf(sq) == -1.0f && cimagf(sq) == 0.0f, "_Complex_I squared is (%g, %g), not (-1, 0)", (double)crealf(sq), (double)cimagf(sq));
	pc_pass();
}
