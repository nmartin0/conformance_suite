/*
 * cacosh of +Inf + iNaN, of -Inf + iNaN and of NaN + iInf is +Inf + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.015
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_PINF, X_NAN, 0 },
	{ X_NINF, X_NAN, X_PINF, X_NAN, 0 },
	{ X_NAN, X_PINF, X_PINF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.015");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
