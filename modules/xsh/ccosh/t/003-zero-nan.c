/*
 * For ccosh, w is the argument. w = +0 + iNaN gives a NaN real part and a
 * zero imaginary part of either sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.003
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_NAN, X_Z, 0 },
};

int main(void)
{
	pc_begin("XSH.ccosh.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccosh", w_ccosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
