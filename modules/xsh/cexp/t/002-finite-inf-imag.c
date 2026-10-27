/*
 * cexp of x + iInf (x finite, including zeros) is NaN + iNaN and the invalid
 * exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.002
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(-1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(2.5L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_P0, X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_N0, X_PINF, X_NAN, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.cexp.002");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
