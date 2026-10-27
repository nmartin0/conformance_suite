/*
 * The imaginary part of cacosh(x + iInf) (x finite, including zeros) is
 * pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.004
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_Q(2), 0 },
	{ X_V(-1.0L), X_PINF, X_PINF, X_Q(2), 0 },
	{ X_P0, X_PINF, X_PINF, X_Q(2), 0 },
	{ X_N0, X_PINF, X_PINF, X_Q(2), 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
