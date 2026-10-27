/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * ccos of -z returns the same value as ccos of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.013
 */
#include "mt.h"

CT_WRAP_CC(ccos)

int main(void)
{
	pc_begin("XSH.ccos.013");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ccos", w_ccos, SYM_EVEN);
	pc_pass();
}
