/*
 * The real part of cacos(+-0 + iNaN) is pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.004
 */
#include "mt.h"

CT_WRAP_CC(cacos)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_Q(2), X_NAN, 0 },
	{ X_N0, X_NAN, X_Q(2), X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.cacos.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacos", w_cacos, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
