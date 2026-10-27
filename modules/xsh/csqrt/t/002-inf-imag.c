/*
 * csqrt of x + iInf is +Inf + iInf for every x: finite, zero, +Inf, -Inf and
 * NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csqrt.002
 */
#include "mt.h"

CT_WRAP_CC(csqrt)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_PINF, 0 },
	{ X_V(-1.0L), X_PINF, X_PINF, X_PINF, 0 },
	{ X_P0, X_PINF, X_PINF, X_PINF, 0 },
	{ X_N0, X_PINF, X_PINF, X_PINF, 0 },
	{ X_PINF, X_PINF, X_PINF, X_PINF, 0 },
	{ X_NINF, X_PINF, X_PINF, X_PINF, 0 },
	{ X_NAN, X_PINF, X_PINF, X_PINF, 0 },
};

int main(void)
{
	pc_begin("XSH.csqrt.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("csqrt", w_csqrt, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
