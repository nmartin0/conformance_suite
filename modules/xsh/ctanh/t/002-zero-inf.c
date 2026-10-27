/*
 * For ctanh, w is the argument. w = +0 + iInf gives a zero real part (either
 * sign) and a NaN imaginary part, and the invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.002
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

static const struct row rows[] = {
	{ X_P0, X_PINF, X_Z, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.ctanh.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctanh", w_ctanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
