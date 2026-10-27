/*
 * For csinh, w is the argument. w = NaN + i0 gives NaN + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csinh.010
 */
#include "mt.h"

CT_WRAP_CC(csinh)

static const struct row rows[] = {
	{ X_NAN, X_P0, X_NAN, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.csinh.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("csinh", w_csinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
