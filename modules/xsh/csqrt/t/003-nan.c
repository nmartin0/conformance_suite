/*
 * csqrt of x + iNaN (x finite, including zeros), of NaN + iy (y finite,
 * including zeros) and of NaN + iNaN is NaN + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csqrt.003
 */
#include "mt.h"

CT_WRAP_CC(csqrt)

static const struct row rows[] = {
	{ X_V(1.0L), X_NAN, X_NAN, X_NAN, 0 },
	{ X_V(-1.0L), X_NAN, X_NAN, X_NAN, 0 },
	{ X_P0, X_NAN, X_NAN, X_NAN, 0 },
	{ X_N0, X_NAN, X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(-1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_P0, X_NAN, X_NAN, 0 },
	{ X_NAN, X_N0, X_NAN, X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.csqrt.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("csqrt", w_csqrt, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
