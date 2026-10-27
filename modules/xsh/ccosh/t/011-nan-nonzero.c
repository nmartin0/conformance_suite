/*
 * For ccosh, w is the argument. w = NaN + iy (y non-zero finite) and w = NaN
 * + iNaN give NaN + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.011
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

static const struct row rows[] = {
	{ X_NAN, X_V(1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(-1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.ccosh.011");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccosh", w_ccosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
