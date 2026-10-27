/*
 * csqrt of +0 + i0 and of -0 + i0 is +0 + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csqrt.001
 */
#include "mt.h"

CT_WRAP_CC(csqrt)

static const struct row rows[] = {
	{ X_P0, X_P0, X_P0, X_P0, 0 },
	{ X_N0, X_P0, X_P0, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.csqrt.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("csqrt", w_csqrt, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
