/*
 * The imaginary part of cacosh(+0 + iNaN) is pi/2 or -pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.006
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_NAN, X_QA(2), 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.006");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
