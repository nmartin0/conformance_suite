/*
 * catanh of the conjugate of z returns the conjugate of catanh of z, part
 * for part (equal values, zero signs included, NaN matching NaN), at
 * ordinary points, at real-axis points where branch cuts lie, and at points
 * with infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catanh.014
 */
#include "mt.h"

CT_WRAP_CC(catanh)

int main(void)
{
	pc_begin("XSH.catanh.014");
	MT_NEED_MXC();
	pc_test();
	ct_sym("catanh", w_catanh, SYM_CONJ);
	pc_pass();
}
