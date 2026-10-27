/*
 * For casinh, w is the argument. w = +Inf + iy (y positive finite: 1, 2.5)
 * gives +Inf + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casinh.005
 */
#include "mt.h"

CT_WRAP_CC(casinh)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_PINF, X_P0, 0 },
	{ X_PINF, X_V(2.5L), X_PINF, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.casinh.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("casinh", w_casinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
