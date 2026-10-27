/*
 * cacos of x + iInf (x finite, including zeros) has imaginary part -Inf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.005
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_ANY, X_NINF, 0 },
	{ X_V(-1.0L), X_PINF, X_ANY, X_NINF, 0 },
	{ X_P0, X_PINF, X_ANY, X_NINF, 0 },
	{ X_N0, X_PINF, X_ANY, X_NINF, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
