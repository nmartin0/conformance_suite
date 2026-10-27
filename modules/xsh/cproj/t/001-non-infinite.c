/*
 * cproj of a value with no infinite part is that value: finite values, zeros
 * with their signs (for a type with negative zero), and values with a NaN
 * part, parts unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cproj.001
 */
#include "mt.h"

CT_WRAP_CC(cproj)

static const struct row rows[] = {
	{ X_V(1.5L), X_V(-2.5L), X_V(1.5L), X_V(-2.5L), 0 },
	{ X_P0, X_P0, X_P0, X_P0, 0 },
	{ X_N0, X_N0, X_N0, X_N0, 0 },
	{ X_P0, X_N0, X_P0, X_N0, 0 },
	{ X_N0, X_P0, X_N0, X_P0, 0 },
	{ X_NAN, X_V(1.0L), X_NAN, X_V(1.0L), 0 },
	{ X_V(1.0L), X_NAN, X_V(1.0L), X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cproj.001");
	pc_test();
	ct_run("cproj", w_cproj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
