/*
 * For catanh, w is the argument. w = x + iInf (x positive finite: 1, 2.5)
 * gives real part +0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.004
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_P0, X_ANY, 0 },
	{ X_V(2.5L), X_PINF, X_P0, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.catanh.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
