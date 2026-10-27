/*
 * The imaginary part of cacosh(+Inf + iInf) is pi/4.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.014
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_PINF, X_Q(1), 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.014");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
