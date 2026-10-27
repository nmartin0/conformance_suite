/*
 * cimag returns the imaginary part of a finite value exactly: 1.5 + 2.5i
 * gives 2.5, -3 + 4i gives 4, 0.25 - 0.5i gives -0.5.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cimag.001
 */
#include "mt.h"

CT_WRAP_CR(cimag)

static const struct row rows[] = {
	{ X_V(1.5L), X_V(2.5L), X_V(2.5L), X_ANY, 0 },
	{ X_V(-3.0L), X_V(4.0L), X_V(4.0L), X_ANY, 0 },
	{ X_V(0.25L), X_V(-0.5L), X_V(-0.5L), X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cimag.001");
	pc_test();
	ct_run("cimag", w_cimag, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
