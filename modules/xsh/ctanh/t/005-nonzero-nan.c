/*
 * For ctanh, w is the argument. w = x + iNaN (x non-zero finite) gives NaN +
 * iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.005
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

static const struct row rows[] = {
	{ X_V(1.0L), X_NAN, X_NAN, X_NAN, 0 },
	{ X_V(-1.0L), X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.ctanh.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctanh", w_ctanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
