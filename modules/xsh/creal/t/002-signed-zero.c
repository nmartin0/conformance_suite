/*
 * creal returns a zero real part with its sign (for a type with negative
 * zero), whatever the imaginary part is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creal.002
 */
#include "mt.h"

CT_WRAP_CR(creal)

static const struct row rows[] = {
	{ X_P0, X_V(1.0L), X_P0, X_ANY, 0 },
	{ X_N0, X_V(1.0L), X_N0, X_ANY, 0 },
	{ X_P0, X_N0, X_P0, X_ANY, 0 },
	{ X_N0, X_N0, X_N0, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.creal.002");
	pc_test();
	ct_run("creal", w_creal, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
