/*
 * For ccosh, w is the argument. w = +Inf + iInf gives an infinite real part
 * of either sign and a NaN imaginary part, and the invalid exception is
 * raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.008
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_INF, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.ccosh.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("ccosh", w_ccosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
