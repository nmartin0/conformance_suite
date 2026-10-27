/*
 * cacos of +Inf + iy (y positive finite: 1, 2.5) is +0 - iInf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.010
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_PINF, X_V(1.0L), X_P0, X_NINF, 0 },
	{ X_PINF, X_V(2.5L), X_P0, X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
