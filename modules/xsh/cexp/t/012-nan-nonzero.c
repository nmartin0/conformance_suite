/*
 * cexp of NaN + iy for a non-zero finite y is NaN + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.012
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_NAN, X_V(1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(-1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(2.5L), X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cexp.012");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
