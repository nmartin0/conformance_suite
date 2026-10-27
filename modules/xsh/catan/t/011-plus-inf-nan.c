/*
 * For catan, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = +Inf + iNaN gives +0 + iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catan.011
 */
#include "mt.h"

CT_WRAP_CC(catan)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_P0, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.catan.011");
	MT_NEED_MXC();
	pc_test();
	ct_run("catan", w_catan, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
