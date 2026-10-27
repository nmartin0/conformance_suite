/*
 * For ccos, in the page's notation w stands for the product i*z; the call is
 * on z = -i*w (parts exchanged, one sign changed). w = +Inf + iInf gives an
 * infinite real part of either sign and a NaN imaginary part, and the
 * invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.008
 */
#include "mt.h"

CT_WRAP_CC(ccos)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_INF, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.ccos.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccos", w_ccos, rows, (int)(sizeof rows / sizeof rows[0]), 2, 0);
	pc_pass();
}
