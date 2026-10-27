/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * csin of -z returns the negation of csin of z, part for part (equal values,
 * zero signs included), at ordinary points, at real-axis points where branch
 * cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csin.013
 */
#include "mt.h"

CT_WRAP_CC(csin)

int main(void)
{
	pc_begin("XSH.csin.013");
	MT_NEED_MXC();
	pc_test();
	ct_sym("csin", w_csin, SYM_ODD);
	pc_pass();
}
