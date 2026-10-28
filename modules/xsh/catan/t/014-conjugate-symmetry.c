/*
 * (The page states this rule in terms of iz, which ranges over every value.)
 * catan of the conjugate of z returns the conjugate of catan of z, part for
 * part (equal values, zero signs included, NaN matching NaN), at ordinary
 * points, at real-axis points where branch cuts lie, and at points with
 * infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catan.014
 */
#include "mt.h"

CT_WRAP_CC(catan)

int main(void)
{
	pc_begin("XSH.catan.014");
	MT_NEED_MXC();
	pc_test();
	ct_sym("catan", w_catan, SYM_CONJ);
	/* A function with no value anywhere keeps any symmetry trivially: at ordinary points the result is a number. */
	{
		int ty, i;

		for (ty = 0; ty < 3; ty++)
			for (i = 0; i < 3; i++) {
				long double x = ct_in(ct_pts[i][0]), y = ct_in(ct_pts[i][1]), rx, ry;

				w_catan(ty, x, y, &rx, &ry);
				pc_check(!isnan(rx) && !isnan(ry), "catan [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), not a number",
					ct_tyname[ty], x, y, rx, ry);
			}
	}
	pc_pass();
}
