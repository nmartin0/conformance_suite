/*
 * clog of -0 + i0 has real part -Inf and clog of +0 + i0 is -Inf + i0; the
 * divide-by-zero exception is raised for both.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.001
 */
#include "mt.h"

CT_WRAP_CC(clog)

static const struct row rows[] = {
	{ X_N0, X_P0, X_NINF, X_ANY, FL_DIV },
	{ X_P0, X_P0, X_NINF, X_P0, FL_DIV },
};

int main(void)
{
	pc_begin("XSH.clog.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("clog", w_clog, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
