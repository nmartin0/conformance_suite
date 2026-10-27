/*
 * cacosh of +0 + iNaN has a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.005
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_P0, X_NAN, X_NAN, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.005");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
