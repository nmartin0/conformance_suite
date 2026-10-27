/*
 * cacosh of the conjugate of z returns the conjugate of cacosh of z, part
 * for part (equal values, zero signs included, NaN matching NaN), at
 * ordinary points, at real-axis points where branch cuts lie, and at points
 * with infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacosh.016
 */
#include "mt.h"

CT_WRAP_CC(cacosh)

int main(void)
{
	pc_begin("XSH.cacosh.016");
	MT_NEED_MXC();
	pc_test();
	ct_sym("cacosh", w_cacosh, SYM_CONJ);
	pc_pass();
}
