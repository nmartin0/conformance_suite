/*
 * carg returns a NaN when either part is a NaN, including NaN with an
 * infinite part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.carg.002
 */
#include "mt.h"

CT_WRAP_CR(carg)

static const struct row rows[] = {
	{ X_NAN, X_V(1.0L), X_NAN, X_ANY, 0 },
	{ X_V(1.0L), X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_PINF, X_NAN, X_ANY, 0 },
	{ X_PINF, X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_P0, X_NAN, X_ANY, 0 },
	{ X_P0, X_NAN, X_NAN, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.carg.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("carg", w_carg, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
