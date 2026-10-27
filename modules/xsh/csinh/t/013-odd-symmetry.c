/*
 * csinh of -z returns the negation of csinh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csinh.013
 */
#include "mt.h"

CT_WRAP_CC(csinh)

int main(void)
{
	pc_begin("XSH.csinh.013");
	MT_NEED_MXC();
	pc_test();
	ct_sym("csinh", w_csinh, SYM_ODD);
	pc_pass();
}
