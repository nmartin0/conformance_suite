/*
 * For catanh, w is the argument. w = +Inf + iNaN gives +0 + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.011
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_P0, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.catanh.011");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
