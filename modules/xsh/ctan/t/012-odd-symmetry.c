/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * ctan of -z returns the negation of ctan of z, part for part (equal values,
 * zero signs included), at ordinary points, at real-axis points where branch
 * cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctan.012
 */
#include "mt.h"

CT_WRAP_CC(ctan)

int main(void)
{
	pc_begin("XSH.ctan.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ctan", w_ctan, SYM_ODD);
	pc_pass();
}
