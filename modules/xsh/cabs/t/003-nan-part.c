/*
 * cabs returns a NaN when a part is a NaN and the other part is not infinite
 * (finite, zero or NaN).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cabs.003
 */
#include "mt.h"

CT_WRAP_CR(cabs)

static const struct row rows[] = {
	{ X_NAN, X_V(1.0L), X_NAN, X_ANY, 0 },
	{ X_V(1.0L), X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_P0, X_NAN, X_ANY, 0 },
	{ X_P0, X_NAN, X_NAN, X_ANY, 0 },
	{ X_NAN, X_V(-2.5L), X_NAN, X_ANY, 0 },
	{ X_V(-2.5L), X_NAN, X_NAN, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cabs.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("cabs", w_cabs, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
