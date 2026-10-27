/*
 * cabs returns +Inf whenever either part is +Inf or -Inf, whatever the other
 * part is: finite, zero, NaN or infinite.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cabs.002
 */
#include "mt.h"

CT_WRAP_CR(cabs)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_PINF, X_ANY, 0 },
	{ X_PINF, X_P0, X_PINF, X_ANY, 0 },
	{ X_PINF, X_NAN, X_PINF, X_ANY, 0 },
	{ X_PINF, X_PINF, X_PINF, X_ANY, 0 },
	{ X_PINF, X_NINF, X_PINF, X_ANY, 0 },
	{ X_NINF, X_V(1.0L), X_PINF, X_ANY, 0 },
	{ X_NINF, X_P0, X_PINF, X_ANY, 0 },
	{ X_NINF, X_NAN, X_PINF, X_ANY, 0 },
	{ X_NINF, X_PINF, X_PINF, X_ANY, 0 },
	{ X_NINF, X_NINF, X_PINF, X_ANY, 0 },
	{ X_V(1.0L), X_PINF, X_PINF, X_ANY, 0 },
	{ X_V(1.0L), X_NINF, X_PINF, X_ANY, 0 },
	{ X_P0, X_PINF, X_PINF, X_ANY, 0 },
	{ X_P0, X_NINF, X_PINF, X_ANY, 0 },
	{ X_NAN, X_PINF, X_PINF, X_ANY, 0 },
	{ X_NAN, X_NINF, X_PINF, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cabs.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("cabs", w_cabs, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
