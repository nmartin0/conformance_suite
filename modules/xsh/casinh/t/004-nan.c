/*
 * For casinh, w is the argument. w = x + iNaN (x finite, including zeros), w
 * = NaN + iy (y non-zero finite) and w = NaN + iNaN give NaN + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casinh.004
 */
#include "mt.h"

CT_WRAP_CC(casinh)

static const struct row rows[] = {
	{ X_V(1.0L), X_NAN, X_NAN, X_NAN, 0 },
	{ X_V(-1.0L), X_NAN, X_NAN, X_NAN, 0 },
	{ X_P0, X_NAN, X_NAN, X_NAN, 0 },
	{ X_N0, X_NAN, X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(-1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.casinh.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("casinh", w_casinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
