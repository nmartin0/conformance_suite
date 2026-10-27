/*
 * conj keeps an infinite or NaN real part, reverses the sign of an infinite
 * imaginary part, and gives a NaN for a NaN imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.conj.003
 */
#include "mt.h"

CT_WRAP_CC(conj)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_PINF, X_NINF, 0 },
	{ X_NINF, X_NINF, X_NINF, X_PINF, 0 },
	{ X_V(1.0L), X_PINF, X_V(1.0L), X_NINF, 0 },
	{ X_PINF, X_V(1.0L), X_PINF, X_V(-1.0L), 0 },
	{ X_NAN, X_V(1.0L), X_NAN, X_V(-1.0L), 0 },
	{ X_V(1.0L), X_NAN, X_V(1.0L), X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
	{ X_NAN, X_PINF, X_NAN, X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.conj.003");
	pc_test();
	ct_run("conj", w_conj, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
