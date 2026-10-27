/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * csin of the conjugate of z returns the conjugate of csin of z, part for
 * part (equal values, zero signs included, NaN matching NaN), at ordinary
 * points, at real-axis points where branch cuts lie, and at points with
 * infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csin.012
 */
#include "mt.h"

CT_WRAP_CC(csin)

int main(void)
{
	pc_begin("XSH.csin.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("csin", w_csin, SYM_CONJ);
	pc_pass();
}
