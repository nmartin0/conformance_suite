/*
 * For ctanh, w is the argument. w = +Inf + iy (y positive finite) gives 1 +
 * i0 sin(2y): real part 1 and a zero imaginary part with the sign of sin(2y)
 * (y = 1 gives +0, y = 2 gives -0).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.006
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_V(1.0L), X_P0, 0 },
	{ X_PINF, X_V(2.0L), X_V(1.0L), X_N0, 0 },
};

int main(void)
{
	pc_begin("XSH.ctanh.006");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctanh", w_ctanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
