/*
 * For ctanh, w is the argument. w = x + iInf (x non-zero finite) gives NaN +
 * iNaN and the invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.003
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(-1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.ctanh.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctanh", w_ctanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
