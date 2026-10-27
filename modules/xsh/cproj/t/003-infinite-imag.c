/*
 * cproj of a value whose imaginary part is +Inf or -Inf, with any real part
 * (finite, zero or NaN), is +Inf with an imaginary part that is +0 or -0
 * following the sign of the infinite part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cproj.003
 */
#include "mt.h"

CT_WRAP_CC(cproj)

static const struct row rows[] = {
	{ X_V(1.5L), X_PINF, X_PINF, X_P0, 0 },
	{ X_V(1.5L), X_NINF, X_PINF, X_N0, 0 },
	{ X_V(-1.5L), X_PINF, X_PINF, X_P0, 0 },
	{ X_V(-1.5L), X_NINF, X_PINF, X_N0, 0 },
	{ X_P0, X_PINF, X_PINF, X_P0, 0 },
	{ X_P0, X_NINF, X_PINF, X_N0, 0 },
	{ X_N0, X_PINF, X_PINF, X_P0, 0 },
	{ X_N0, X_NINF, X_PINF, X_N0, 0 },
	{ X_NAN, X_PINF, X_PINF, X_P0, 0 },
	{ X_NAN, X_NINF, X_PINF, X_N0, 0 },
};

int main(void)
{
	pc_begin("XSH.cproj.003");
	pc_test();
	ct_run("cproj", w_cproj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
