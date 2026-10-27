/*
 * For ccosh, w is the argument. w = +0 + i0 gives 1 + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.001
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

static const struct row rows[] = {
	{ X_P0, X_P0, X_V(1.0L), X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.ccosh.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccosh", w_ccosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
