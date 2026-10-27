/*
 * For ccos, in the page's notation w stands for the product i*z; the call is
 * on z = -i*w (parts exchanged, one sign changed). w = +Inf + iNaN gives
 * +Inf + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.009
 */
#include "mt.h"

CT_WRAP_CC(ccos)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_PINF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.ccos.009");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccos", w_ccos, rows, (int)(sizeof rows / sizeof rows[0]), 2, 0);
	pc_pass();
}
