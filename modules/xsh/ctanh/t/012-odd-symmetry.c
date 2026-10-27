/*
 * ctanh of -z returns the negation of ctanh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctanh.012
 */
#include "mt.h"

CT_WRAP_CC(ctanh)

int main(void)
{
	pc_begin("XSH.ctanh.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ctanh", w_ctanh, SYM_ODD);
	pc_pass();
}
