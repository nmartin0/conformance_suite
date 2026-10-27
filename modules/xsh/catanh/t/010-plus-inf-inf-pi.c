/*
 * For catanh, w is the argument. w = +Inf + iInf gives imaginary part pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.010
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_P0, X_Q(2), 0 },
};

int main(void)
{
	pc_begin("XSH.catanh.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
