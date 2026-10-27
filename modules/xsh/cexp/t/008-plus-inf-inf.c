/*
 * cexp of +Inf + iInf has an infinite real part (either sign) and a NaN
 * imaginary part, and the invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cexp.008
 */
#include "mt.h"

CT_WRAP_CC(cexp)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_INF, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.cexp.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("cexp", w_cexp, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
