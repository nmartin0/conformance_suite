/*
 * csqrt of -Inf + iNaN is a NaN real part with an infinite imaginary part of
 * either sign, and csqrt of +Inf + iNaN is +Inf + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csqrt.005
 */
#include "mt.h"

CT_WRAP_CC(csqrt)

static const struct row rows[] = {
	{ X_NINF, X_NAN, X_NAN, X_INF, 0 },
	{ X_PINF, X_NAN, X_PINF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.csqrt.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("csqrt", w_csqrt, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
