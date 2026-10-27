/*
 * csqrt of -Inf + iy (y positive finite: 1, 2.5) is +0 + iInf, and csqrt of
 * +Inf + iy is +Inf + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csqrt.004
 */
#include "mt.h"

CT_WRAP_CC(csqrt)

static const struct row rows[] = {
	{ X_NINF, X_V(1.0L), X_P0, X_PINF, 0 },
	{ X_NINF, X_V(2.5L), X_P0, X_PINF, 0 },
	{ X_PINF, X_V(1.0L), X_PINF, X_P0, 0 },
	{ X_PINF, X_V(2.5L), X_PINF, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.csqrt.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("csqrt", w_csqrt, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
