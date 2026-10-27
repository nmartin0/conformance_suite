/*
 * clog of +Inf + iy (y positive finite: 1, 2.5) is +Inf + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.008
 */
#include "mt.h"

CT_WRAP_CC(clog)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_PINF, X_P0, 0 },
	{ X_PINF, X_V(2.5L), X_PINF, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.clog.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("clog", w_clog, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
