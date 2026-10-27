/*
 * cimag returns an infinite or NaN imaginary part as it is, and the
 * imaginary part when the real part is infinite or NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cimag.003
 */
#include "mt.h"

CT_WRAP_CR(cimag)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_ANY, 0 },
	{ X_V(1.0L), X_NINF, X_NINF, X_ANY, 0 },
	{ X_V(1.0L), X_NAN, X_NAN, X_ANY, 0 },
	{ X_PINF, X_V(1.0L), X_V(1.0L), X_ANY, 0 },
	{ X_NAN, X_V(1.0L), X_V(1.0L), X_ANY, 0 },
	{ X_NAN, X_PINF, X_PINF, X_ANY, 0 },
	{ X_PINF, X_NAN, X_NAN, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cimag.003");
	pc_test();
	ct_run("cimag", w_cimag, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
