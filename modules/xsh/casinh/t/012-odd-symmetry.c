/*
 * casinh of -z returns the negation of casinh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casinh.012
 */
#include "mt.h"

CT_WRAP_CC(casinh)

int main(void)
{
	pc_begin("XSH.casinh.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("casinh", w_casinh, SYM_ODD);
	pc_pass();
}
