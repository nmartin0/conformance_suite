/*
 * cexp of -Inf + iy (y finite) is a positive zero times cos(y) + i sin(y):
 * each part is a zero whose sign is that of the cosine or sine (y = 1, 2, 4,
 * 5, +0, -0).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.005
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_NINF, X_V(1.0L), X_P0, X_P0, 0 },
	{ X_NINF, X_V(2.0L), X_N0, X_P0, 0 },
	{ X_NINF, X_V(4.0L), X_N0, X_N0, 0 },
	{ X_NINF, X_V(5.0L), X_P0, X_N0, 0 },
	{ X_NINF, X_P0, X_P0, X_P0, 0 },
	{ X_NINF, X_N0, X_P0, X_N0, 0 },
};

int main(void)
{
	pc_begin("XSH.cexp.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
