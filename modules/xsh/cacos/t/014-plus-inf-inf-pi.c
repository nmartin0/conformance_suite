/*
 * The real part of cacos(+Inf + iInf) is pi/4.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.014
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_PINF, X_PINF, X_Q(1), X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.014");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
