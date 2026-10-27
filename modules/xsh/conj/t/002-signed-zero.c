/*
 * conj reverses the sign of a zero imaginary part and leaves the sign of a
 * zero real part alone (for a type with negative zero): +0 + i0 gives +0 -
 * i0, -0 + i0 gives -0 - i0, +0 - i0 gives +0 + i0, -0 - i0 gives -0 + i0,
 * 1.5 + i0 gives 1.5 - i0, +0 + 2.5i gives +0 - 2.5i.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.conj.002
 */
#include "mt.h"

CT_WRAP_CC(conj)

static const struct row rows[] = {
	{ X_P0, X_P0, X_P0, X_N0, 0 },
	{ X_N0, X_P0, X_N0, X_N0, 0 },
	{ X_P0, X_N0, X_P0, X_P0, 0 },
	{ X_N0, X_N0, X_N0, X_P0, 0 },
	{ X_V(1.5L), X_P0, X_V(1.5L), X_N0, 0 },
	{ X_P0, X_V(2.5L), X_P0, X_V(-2.5L), 0 },
};

int main(void)
{
	pc_begin("XSH.conj.002");
	pc_test();
	ct_run("conj", w_conj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
