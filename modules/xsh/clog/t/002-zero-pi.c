/*
 * The imaginary part of clog(-0 + i0) is pi.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.002
 */
#include "mt.h"

CT_WRAP_CC(clog)

static const struct row rows[] = {
	{ X_N0, X_P0, X_NINF, X_Q(4), FL_DIV },
};

int main(void)
{
	pc_begin("XSH.clog.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("clog", w_clog, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
