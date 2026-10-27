/*
 * clog of x + iInf (x finite, including zeros) has real part +Inf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.003
 */
#include "mt.h"

CT_WRAP_CC(clog)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_ANY, 0 },
	{ X_V(-1.0L), X_PINF, X_PINF, X_ANY, 0 },
	{ X_P0, X_PINF, X_PINF, X_ANY, 0 },
	{ X_N0, X_PINF, X_PINF, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.clog.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("clog", w_clog, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
