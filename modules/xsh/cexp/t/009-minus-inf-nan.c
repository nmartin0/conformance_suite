/*
 * cexp of -Inf + iNaN is a zero in each part, with any signs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.009
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_NINF, X_NAN, X_Z, X_Z, 0 },
};

int main(void)
{
	pc_begin("XSH.cexp.009");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
