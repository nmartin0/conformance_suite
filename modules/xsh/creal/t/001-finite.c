/*
 * creal returns the real part of a finite value exactly: 1.5 + 2.5i gives
 * 1.5, -3 + 4i gives -3, 0.25 - 0.5i gives 0.25.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creal.001
 */
#include "mt.h"

CT_WRAP_CR(creal)

static const struct row rows[] = {
	{ X_V(1.5L), X_V(2.5L), X_V(1.5L), X_ANY, 0 },
	{ X_V(-3.0L), X_V(4.0L), X_V(-3.0L), X_ANY, 0 },
	{ X_V(0.25L), X_V(-0.5L), X_V(0.25L), X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.creal.001");
	pc_test();
	ct_run("creal", w_creal, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
