/*
 * For csinh, w is the argument. w = +Inf + iy (y positive finite) gives
 * positive infinity times cos(y) + i sin(y): each part is an infinity whose
 * sign is that of the cosine or sine (y = 1, 2, 4, 5).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csinh.007
 */
#include "mt.h"

CT_WRAP_CC(csinh)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_PINF, X_PINF, 0 },
	{ X_PINF, X_V(2.0L), X_NINF, X_PINF, 0 },
	{ X_PINF, X_V(4.0L), X_NINF, X_NINF, 0 },
	{ X_PINF, X_V(5.0L), X_PINF, X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.csinh.007");
	MT_NEED_MXC();
	pc_test();
	ct_run("csinh", w_csinh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
