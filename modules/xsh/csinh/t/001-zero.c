/*
 * For csinh, w is the argument. w = +0 + i0 gives +0 + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csinh.001
 */
#include "mt.h"

CT_WRAP_CC(csinh)

static const struct row rows[] = {
	{ X_P0, X_P0, X_P0, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.csinh.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("csinh", w_csinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
