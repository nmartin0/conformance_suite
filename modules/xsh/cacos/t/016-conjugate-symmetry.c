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
	/* A function with no value anywhere keeps any symmetry trivially: at ordinary points the result is a number. */
	{
		int ty, i;

		for (ty = 0; ty < 3; ty++)
			for (i = 0; i < 3; i++) {
				long double x = ct_in(ct_pts[i][0]), y = ct_in(ct_pts[i][1]), rx, ry;

				w_cacos(ty, x, y, &rx, &ry);
				pc_check(!isnan(rx) && !isnan(ry), "cacos [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), not a number",
					ct_tyname[ty], x, y, rx, ry);
			}
	}
	pc_pass();
}
