/*
 * M_EGAMMA, M_PHI, M_1_SQRTPI, M_SQRT3 and M_SQRT1_3 have type double
 *   and the right values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.math.h.014
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
	ROW(M_EGAMMA, 0.5772156649015328606065120900824024310422L),
	ROW(M_PHI, 1.6180339887498948482045868343656381177203L),
	ROW(M_1_SQRTPI, 0.5641895835477562869480794515607725858441L),
	ROW(M_SQRT3, 1.7320508075688772935274463415058723669428L),
	ROW(M_SQRT1_3, 0.5773502691896257645091487805019574556476L),
};

int main(void)
{
	size_t i;

	pc_begin("XBD.math.h.014");
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		long double diff = fabsl((long double)rows[i].value - rows[i].ref);

		/* accurate to the precision of the type: within two units of the last place */
		pc_check(diff <= 2 * DBL_EPSILON * rows[i].ref,
		    "%s is off by %Lg from the true value %.30Lg", rows[i].name, diff, rows[i].ref);
	}
	pc_check(IS_T(M_EGAMMA, double), "M_EGAMMA does not have type double");
	pc_check((double)M_EGAMMA == M_EGAMMA, "(double)M_EGAMMA differs from M_EGAMMA");
	pc_check(IS_T(M_PHI, double), "M_PHI does not have type double");
	pc_check((double)M_PHI == M_PHI, "(double)M_PHI differs from M_PHI");
	pc_check(IS_T(M_1_SQRTPI, double), "M_1_SQRTPI does not have type double");
	pc_check((double)M_1_SQRTPI == M_1_SQRTPI, "(double)M_1_SQRTPI differs from M_1_SQRTPI");
	pc_check(IS_T(M_SQRT3, double), "M_SQRT3 does not have type double");
	pc_check((double)M_SQRT3 == M_SQRT3, "(double)M_SQRT3 differs from M_SQRT3");
	pc_check(IS_T(M_SQRT1_3, double), "M_SQRT1_3 does not have type double");
	pc_check((double)M_SQRT1_3 == M_SQRT1_3, "(double)M_SQRT1_3 differs from M_SQRT1_3");
	pc_pass();
}
