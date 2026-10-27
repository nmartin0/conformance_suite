/*
 * The long-standing M_ constants have type double and are accurate to
 *   the precision of double.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.013
 */
#include <math.h>
#include <float.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

struct row {
	const char *name;
	double value;
	long double ref;
};
#define ROW(m, r) { #m, m, r }

static const struct row rows[] = {
	ROW(M_E, 2.7182818284590452353602874713526624977572L),
	ROW(M_LOG2E, 1.4426950408889634073599246810018921374266L),
	ROW(M_LOG10E, 0.4342944819032518276511289189166050822944L),
	ROW(M_LN2, 0.6931471805599453094172321214581765680755L),
	ROW(M_LN10, 2.3025850929940456840179914546843642076011L),
	ROW(M_PI, 3.1415926535897932384626433832795028841972L),
	ROW(M_PI_2, 1.5707963267948966192313216916397514420986L),
	ROW(M_PI_4, 0.7853981633974483096156608458198757210493L),
	ROW(M_1_PI, 0.3183098861837906715377675267450287240689L),
	ROW(M_2_PI, 0.6366197723675813430755350534900574481378L),
	ROW(M_2_SQRTPI, 1.1283791670955125738961589031215451716881L),
	ROW(M_SQRT2, 1.4142135623730950488016887242096980785697L),
	ROW(M_SQRT1_2, 0.7071067811865475244008443621048490392848L),
};

int main(void)
{
	size_t i;

	pc_begin("XBD.math.h.013");
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		long double diff = fabsl((long double)rows[i].value - rows[i].ref);

		/* accurate to the precision of the type: within two units of the last place */
		pc_check(diff <= 2 * DBL_EPSILON * rows[i].ref,
		    "%s is off by %Lg from the true value %.30Lg", rows[i].name, diff, rows[i].ref);
	}
	pc_check(IS_T(M_E, double), "M_E does not have type double");
	pc_check((double)M_E == M_E, "(double)M_E differs from M_E");
	pc_check(IS_T(M_LOG2E, double), "M_LOG2E does not have type double");
	pc_check((double)M_LOG2E == M_LOG2E, "(double)M_LOG2E differs from M_LOG2E");
	pc_check(IS_T(M_LOG10E, double), "M_LOG10E does not have type double");
	pc_check((double)M_LOG10E == M_LOG10E, "(double)M_LOG10E differs from M_LOG10E");
	pc_check(IS_T(M_LN2, double), "M_LN2 does not have type double");
	pc_check((double)M_LN2 == M_LN2, "(double)M_LN2 differs from M_LN2");
	pc_check(IS_T(M_LN10, double), "M_LN10 does not have type double");
	pc_check((double)M_LN10 == M_LN10, "(double)M_LN10 differs from M_LN10");
	pc_check(IS_T(M_PI, double), "M_PI does not have type double");
	pc_check((double)M_PI == M_PI, "(double)M_PI differs from M_PI");
	pc_check(IS_T(M_PI_2, double), "M_PI_2 does not have type double");
	pc_check((double)M_PI_2 == M_PI_2, "(double)M_PI_2 differs from M_PI_2");
	pc_check(IS_T(M_PI_4, double), "M_PI_4 does not have type double");
	pc_check((double)M_PI_4 == M_PI_4, "(double)M_PI_4 differs from M_PI_4");
	pc_check(IS_T(M_1_PI, double), "M_1_PI does not have type double");
	pc_check((double)M_1_PI == M_1_PI, "(double)M_1_PI differs from M_1_PI");
	pc_check(IS_T(M_2_PI, double), "M_2_PI does not have type double");
	pc_check((double)M_2_PI == M_2_PI, "(double)M_2_PI differs from M_2_PI");
	pc_check(IS_T(M_2_SQRTPI, double), "M_2_SQRTPI does not have type double");
	pc_check((double)M_2_SQRTPI == M_2_SQRTPI, "(double)M_2_SQRTPI differs from M_2_SQRTPI");
	pc_check(IS_T(M_SQRT2, double), "M_SQRT2 does not have type double");
	pc_check((double)M_SQRT2 == M_SQRT2, "(double)M_SQRT2 differs from M_SQRT2");
	pc_check(IS_T(M_SQRT1_2, double), "M_SQRT1_2 does not have type double");
	pc_check((double)M_SQRT1_2 == M_SQRT1_2, "(double)M_SQRT1_2 differs from M_SQRT1_2");
	pc_pass();
}
