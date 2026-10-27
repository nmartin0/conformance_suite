/*
 * catanh of -z returns the negation of catanh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.015
 */
#include "mt.h"

CT_WRAP_CC(catanh)

int main(void)
{
	pc_begin("XSH.catanh.015");
	MT_NEED_MXC();
	pc_test();
	ct_sym("catanh", w_catanh, SYM_ODD);
	pc_pass();
}
