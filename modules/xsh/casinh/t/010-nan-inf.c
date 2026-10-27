/*
 * For casinh, w is the argument. w = NaN + iInf gives an infinite real part
 * of either sign and a NaN imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casinh.010
 */
#include "mt.h"

CT_WRAP_CC(casinh)

static const struct row rows[] = {
	{ X_NAN, X_PINF, X_INF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.casinh.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("casinh", w_casinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
