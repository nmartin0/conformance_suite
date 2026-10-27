/*
 * For ccosh, w is the argument. w = +Inf + iNaN gives +Inf + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.009
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_PINF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.ccosh.009");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccosh", w_ccosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
