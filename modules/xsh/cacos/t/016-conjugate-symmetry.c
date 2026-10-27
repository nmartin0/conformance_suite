/*
 * cacos of the conjugate of z returns the conjugate of cacos of z, part for
 * part (equal values, zero signs included, NaN matching NaN), at ordinary
 * points, at real-axis points where branch cuts lie, and at points with
 * infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cacos.016
 */
#include "mt.h"

CT_WRAP_CC(cacos)

int main(void)
{
	pc_begin("XSH.cacos.016");
	MT_NEED_MXC();
	pc_test();
	ct_sym("cacos", w_cacos, SYM_CONJ);
	pc_pass();
}
