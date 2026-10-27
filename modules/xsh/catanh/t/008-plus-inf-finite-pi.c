/*
 * For catanh, w is the argument. w = +Inf + iy (y positive finite: 1, 2.5)
 * gives imaginary part pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.008
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_P0, X_Q(2), 0 },
	{ X_PINF, X_V(2.5L), X_P0, X_Q(2), 0 },
};

int main(void)
{
	pc_begin("XSH.catanh.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
