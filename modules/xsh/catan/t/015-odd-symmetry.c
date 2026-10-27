/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * catan of -z returns the negation of catan of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catan.015
 */
#include "mt.h"

CT_WRAP_CC(catan)

int main(void)
{
	pc_begin("XSH.catan.015");
	MT_NEED_MXC();
	pc_test();
	ct_sym("catan", w_catan, SYM_ODD);
	pc_pass();
}
