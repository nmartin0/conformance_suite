/*
 * For catanh, w is the argument. w = +1 + i0 gives +Inf + i0 and the
 * divide-by-zero exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.003
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_V(1.0L), X_P0, X_PINF, X_P0, FL_DIV },
};

int main(void)
{
	pc_begin("XSH.catanh.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
