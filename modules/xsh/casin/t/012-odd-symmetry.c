/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * casin of -z returns the negation of casin of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casin.012
 */
#include "mt.h"

CT_WRAP_CC(casin)

int main(void)
{
	pc_begin("XSH.casin.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("casin", w_casin, SYM_ODD);
	pc_pass();
}
