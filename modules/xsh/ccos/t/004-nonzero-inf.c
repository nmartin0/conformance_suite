/*
 * For ccos, in the page's notation w stands for the product i*z; the call is
 * on z = -i*w (parts exchanged, one sign changed). w = x + iInf (x non-zero
 * finite) gives NaN + iNaN and the invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.004
 */
#include "mt.h"

CT_WRAP_CC(ccos)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(-1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.ccos.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccos", w_ccos, rows, (int)(sizeof rows / sizeof rows[0]), 2, 0);
	pc_pass();
}
