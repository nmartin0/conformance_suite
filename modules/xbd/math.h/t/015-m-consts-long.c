/*
 * The M_ constants with suffix l have type long double and are accurate
 *   to the precision of long double.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.015
 */
#include <math.h>
#include <float.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

struct row {
	const char *name;
	long double value;
	long double ref;
};
#define ROW(m, r) { #m, m, r }

static const struct row rows[] = {
	ROW(M_El, 2.7182818284590452353602874713526624977572L),
	ROW(M_EGAMMAl, 0.5772156649015328606065120900824024310422L),
	ROW(M_LOG2El, 1.4426950408889634073599246810018921374266L),
	ROW(M_LOG10El, 0.4342944819032518276511289189166050822944L),
	ROW(M_LN2l, 0.6931471805599453094172321214581765680755L),
	ROW(M_LN10l, 2.3025850929940456840179914546843642076011L),
	ROW(M_PHIl, 1.6180339887498948482045868343656381177203L),
	ROW(M_PIl, 3.1415926535897932384626433832795028841972L),
	ROW(M_PI_2l, 1.5707963267948966192313216916397514420986L),
	ROW(M_PI_4l, 0.7853981633974483096156608458198757210493L),
	ROW(M_1_PIl, 0.3183098861837906715377675267450287240689L),
	ROW(M_1_SQRTPIl, 0.5641895835477562869480794515607725858441L),
	ROW(M_2_PIl, 0.6366197723675813430755350534900574481378L),
	ROW(M_2_SQRTPIl, 1.1283791670955125738961589031215451716881L),
	ROW(M_SQRT2l, 1.4142135623730950488016887242096980785697L),
	ROW(M_SQRT3l, 1.7320508075688772935274463415058723669428L),
	ROW(M_SQRT1_2l, 0.7071067811865475244008443621048490392848L),
	ROW(M_SQRT1_3l, 0.5773502691896257645091487805019574556476L),
};

int main(void)
{
	size_t i;

	pc_begin("XBD.math.h.015");
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		long double diff = fabsl((long double)rows[i].value - rows[i].ref);

		/* accurate to the precision of the type: within two units of the last place */
		pc_check(diff <= 2 * LDBL_EPSILON * rows[i].ref,
		    "%s is off by %Lg from the true value %.30Lg", rows[i].name, diff, rows[i].ref);
	}
	pc_check(IS_T(M_El, long double), "M_El does not have type long double");
	pc_check(IS_T(M_EGAMMAl, long double), "M_EGAMMAl does not have type long double");
	pc_check(IS_T(M_LOG2El, long double), "M_LOG2El does not have type long double");
	pc_check(IS_T(M_LOG10El, long double), "M_LOG10El does not have type long double");
	pc_check(IS_T(M_LN2l, long double), "M_LN2l does not have type long double");
	pc_check(IS_T(M_LN10l, long double), "M_LN10l does not have type long double");
	pc_check(IS_T(M_PHIl, long double), "M_PHIl does not have type long double");
	pc_check(IS_T(M_PIl, long double), "M_PIl does not have type long double");
	pc_check(IS_T(M_PI_2l, long double), "M_PI_2l does not have type long double");
	pc_check(IS_T(M_PI_4l, long double), "M_PI_4l does not have type long double");
	pc_check(IS_T(M_1_PIl, long double), "M_1_PIl does not have type long double");
	pc_check(IS_T(M_1_SQRTPIl, long double), "M_1_SQRTPIl does not have type long double");
	pc_check(IS_T(M_2_PIl, long double), "M_2_PIl does not have type long double");
	pc_check(IS_T(M_2_SQRTPIl, long double), "M_2_SQRTPIl does not have type long double");
	pc_check(IS_T(M_SQRT2l, long double), "M_SQRT2l does not have type long double");
	pc_check(IS_T(M_SQRT3l, long double), "M_SQRT3l does not have type long double");
	pc_check(IS_T(M_SQRT1_2l, long double), "M_SQRT1_2l does not have type long double");
	pc_check(IS_T(M_SQRT1_3l, long double), "M_SQRT1_3l does not have type long double");
	pc_pass();
}
