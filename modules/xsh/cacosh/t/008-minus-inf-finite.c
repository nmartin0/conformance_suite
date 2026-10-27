/*
 * cacosh of -Inf + iy (y positive finite: 1, 2.5) has real part +Inf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.008
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

static const struct row rows[] = {
	{ X_NINF, X_V(1.0L), X_PINF, X_ANY, 0 },
	{ X_NINF, X_V(2.5L), X_PINF, X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.cacosh.008");
	MT_NEED_MXC();
	pc_test();
	ct_run("cacosh", w_cacosh, rows, (int)(sizeof rows / sizeof rows[0]), 0, 0);
	pc_pass();
}
