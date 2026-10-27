/*
 * carg returns a zero whose sign is that of the imaginary part when the real
 * part is +0, positive finite or +Inf with the imaginary part a zero or a
 * positive or negative finite value (+0 + i0 gives +0, +0 - i0 gives -0, 2.5
 * +- i0 gives +-0, +Inf +- iy gives +-0 for y = 1, 2.5).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.carg.001
 */
#include "mt.h"

CT_WRAP_CR(carg)

static const struct row rows[] = {
	{ X_P0, X_P0, X_P0, X_ANY, 0 },
	{ X_P0, X_N0, X_N0, X_ANY, 0 },
	{ X_V(2.5L), X_P0, X_P0, X_ANY, 0 },
	{ X_V(2.5L), X_N0, X_N0, X_ANY, 0 },
	{ X_PINF, X_V(1.0L), X_P0, X_ANY, 0 },
	{ X_PINF, X_V(-1.0L), X_N0, X_ANY, 0 },
	{ X_PINF, X_V(2.5L), X_P0, X_ANY, 0 },
	{ X_PINF, X_V(-2.5L), X_N0, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.carg.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("carg", w_carg, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
