/*
 * cproj of a value whose real part is +Inf or -Inf, with any imaginary part
 * (finite, zero, infinite or NaN), is +Inf with an imaginary part that is a
 * zero carrying the sign of the imaginary part (either sign for a NaN).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cproj.002
 */
#include "mt.h"

CT_WRAP_CC(cproj)

static const struct row rows[] = {
	{ X_PINF, X_V(1.5L), X_PINF, X_P0, 0 },
	{ X_PINF, X_V(-1.5L), X_PINF, X_N0, 0 },
	{ X_PINF, X_P0, X_PINF, X_P0, 0 },
	{ X_PINF, X_N0, X_PINF, X_N0, 0 },
	{ X_PINF, X_PINF, X_PINF, X_P0, 0 },
	{ X_PINF, X_NINF, X_PINF, X_N0, 0 },
	{ X_PINF, X_NAN, X_PINF, X_Z, 0 },
	{ X_NINF, X_V(1.5L), X_PINF, X_P0, 0 },
	{ X_NINF, X_V(-1.5L), X_PINF, X_N0, 0 },
	{ X_NINF, X_P0, X_PINF, X_P0, 0 },
	{ X_NINF, X_N0, X_PINF, X_N0, 0 },
	{ X_NINF, X_PINF, X_PINF, X_P0, 0 },
	{ X_NINF, X_NINF, X_PINF, X_N0, 0 },
	{ X_NINF, X_NAN, X_PINF, X_Z, 0 },
};

int main(void)
{
	pc_begin("XSH.cproj.002");
	pc_test();
	ct_run("cproj", w_cproj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
