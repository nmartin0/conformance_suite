/*
 * creal returns an infinite or NaN real part as it is, and the real part
 * when the imaginary part is infinite or NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creal.003
 */
#include "mt.h"

CT_WRAP_CR(creal)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_PINF, X_ANY, 0 },
	{ X_NINF, X_V(1.0L), X_NINF, X_ANY, 0 },
	{ X_NAN, X_V(1.0L), X_NAN, X_ANY, 0 },
	{ X_V(1.0L), X_PINF, X_V(1.0L), X_ANY, 0 },
	{ X_V(1.0L), X_NAN, X_V(1.0L), X_ANY, 0 },
	{ X_PINF, X_NAN, X_PINF, X_ANY, 0 },
	{ X_NAN, X_NINF, X_NAN, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.creal.003");
	pc_test();
	ct_run("creal", w_creal, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
