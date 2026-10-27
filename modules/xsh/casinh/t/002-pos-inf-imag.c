/*
 * For casinh, w is the argument. w = x + iInf (x positive finite: 1, 2.5)
 * gives a result with real part +Inf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casinh.002
 */
#include "mt.h"

CT_WRAP_CC(casinh)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_ANY, 0 },
	{ X_V(2.5L), X_PINF, X_PINF, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.casinh.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("casinh", w_casinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
