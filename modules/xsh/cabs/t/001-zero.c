/*
 * cabs of any zero with any signs of its parts (+0 + i0, -0 + i0, +0 - i0,
 * -0 - i0) returns +0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cabs.001
 */
#include "mt.h"

CT_WRAP_CR(cabs)

static const struct row rows[] = {
	{ X_P0, X_P0, X_P0, X_ANY, 0 },
	{ X_P0, X_N0, X_P0, X_ANY, 0 },
	{ X_N0, X_P0, X_P0, X_ANY, 0 },
	{ X_N0, X_N0, X_P0, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cabs.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("cabs", w_cabs, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
