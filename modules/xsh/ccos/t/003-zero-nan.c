/*
 * For ccos, in the page's notation w stands for the product i*z; the call is
 * on z = -i*w (parts exchanged, one sign changed). w = +0 + iNaN gives a NaN
 * real part and a zero imaginary part of either sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.003
 */
#include "mt.h"

CT_WRAP_CC(ccos)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_NAN, X_Z, 0 },
};

int main(void)
{
	pc_begin("XSH.ccos.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccos", w_ccos, rows, (int)(sizeof rows / sizeof rows[0]), 2, 0);
	pc_pass();
}
