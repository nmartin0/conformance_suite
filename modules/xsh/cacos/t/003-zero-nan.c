/*
 * cacos of +0 + iNaN and of -0 + iNaN has a NaN imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.003
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_ANY, X_NAN, 0 },
	{ X_N0, X_NAN, X_ANY, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
