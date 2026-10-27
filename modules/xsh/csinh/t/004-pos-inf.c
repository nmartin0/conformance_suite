/*
 * For csinh, w is the argument. w = x + iInf (x positive finite: 1, 2.5)
 * gives NaN + iNaN and the invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csinh.004
 */
#include "mt.h"

CT_WRAP_CC(csinh)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(2.5L), X_PINF, X_NAN, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.csinh.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("csinh", w_csinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
