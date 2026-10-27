/*
 * cexp of +Inf + iNaN has an infinite real part (either sign) and a NaN
 * imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.010
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_PINF, X_NAN, X_INF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cexp.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
