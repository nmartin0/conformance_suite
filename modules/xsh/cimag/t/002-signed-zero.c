/*
 * cimag returns a zero imaginary part with its sign (for a type with
 * negative zero), whatever the real part is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cimag.002
 */
#include "mt.h"

CT_WRAP_CR(cimag)

static const struct row rows[] = {
	{ X_V(1.0L), X_P0, X_P0, X_ANY, 0 },
	{ X_V(1.0L), X_N0, X_N0, X_ANY, 0 },
	{ X_N0, X_P0, X_P0, X_ANY, 0 },
	{ X_N0, X_N0, X_N0, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cimag.002");
	pc_test();
	ct_run("cimag", w_cimag, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
