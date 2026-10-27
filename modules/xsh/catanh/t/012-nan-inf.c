/*
 * For catanh, w is the argument. w = NaN + iInf gives a zero real part of
 * either sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.012
 */
#include "mt.h"

CT_WRAP_CC(catanh)

static const struct row rows[] = {
	{ X_NAN, X_PINF, X_Z, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.catanh.012");
	MT_NEED_MXC();
	pc_test();
	ct_run("catanh", w_catanh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
