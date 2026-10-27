/*
 * ccosh of -z returns the same value as ccosh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.013
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

int main(void)
{
	pc_begin("XSH.ccosh.013");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ccosh", w_ccosh, SYM_EVEN);
	pc_pass();
}
