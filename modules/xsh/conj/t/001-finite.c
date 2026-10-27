/*
 * conj of a finite value keeps the real part and reverses the sign of the
 * imaginary part: 1.5 + 2.5i gives 1.5 - 2.5i, -1 - 3i gives -1 + 3i, 0.25 -
 * 0.5i gives 0.25 + 0.5i.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.conj.001
 */
#include "mt.h"

CT_WRAP_CC(conj)

static const struct row rows[] = {
	{ X_V(1.5L), X_V(2.5L), X_V(1.5L), X_V(-2.5L), 0 },
	{ X_V(-1.0L), X_V(-3.0L), X_V(-1.0L), X_V(3.0L), 0 },
	{ X_V(0.25L), X_V(-0.5L), X_V(0.25L), X_V(0.5L), 0 },
};

int main(void)
{
	pc_begin("XSH.conj.001");
	pc_test();
	ct_run("conj", w_conj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
