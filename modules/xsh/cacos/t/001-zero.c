/*
 * cacos of +0 + i0 and of -0 + i0 has imaginary part -0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.001
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_P0, X_P0, X_ANY, X_N0, 0 },
	{ X_N0, X_P0, X_ANY, X_N0, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.001");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
