/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * ccos of the conjugate of z returns the conjugate of ccos of z, part for
 * part (equal values, zero signs included, NaN matching NaN), at ordinary
 * points, at real-axis points where branch cuts lie, and at points with
 * infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccos.012
 */
#include "mt.h"

CT_WRAP_CC(ccos)

int main(void)
{
	pc_begin("XSH.ccos.012");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ccos", w_ccos, SYM_CONJ);
	pc_pass();
}
