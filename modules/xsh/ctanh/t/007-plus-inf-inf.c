/*
 * For ctanh, w is the argument. w = +Inf + iInf gives real part 1 and a zero
 * imaginary part of either sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.007
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_V(1.0L), X_Z, 0 },
};

int main(void)
{
	pc_begin("XSH.ctanh.007");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctanh", w_ctanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
