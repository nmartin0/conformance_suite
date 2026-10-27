/*
 * The imaginary part of clog(-Inf + iInf) is 3pi/4.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.010
 */
#include "mt.h"

CT_WRAP_CC(clog)

static const struct row rows[] = {
	{ X_NINF, X_PINF, X_PINF, X_Q(3), 0 },
};

int main(void)
{
	pc_begin("XSH.clog.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("clog", w_clog, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
