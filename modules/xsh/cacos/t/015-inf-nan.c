/*
 * cacos of +Inf + iNaN and of -Inf + iNaN has a NaN real part and an
 * infinite imaginary part of either sign; cacos of NaN + iInf is NaN - iInf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.015
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_NAN, X_INF, 0 },
	{ X_NINF, X_NAN, X_NAN, X_INF, 0 },
	{ X_NAN, X_PINF, X_NAN, X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.015");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
