/*
 * clog of the conjugate of z returns the conjugate of clog of z, part for
 * part (equal values, zero signs included, NaN matching NaN), at ordinary
 * points, at real-axis points where branch cuts lie, and at points with
 * infinite parts and with a NaN real part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clog.014
 */
#include "mt.h"

CT_WRAP_CC(clog)

int main(void)
{
	pc_begin("XSH.clog.014");
	MT_NEED_MXC();
	pc_test();
	ct_sym("clog", w_clog, SYM_CONJ);
	/* A function with no value anywhere keeps any symmetry trivially: at ordinary points the result is a number. */
	{
		int ty, i;

		for (ty = 0; ty < 3; ty++)
			for (i = 0; i < 3; i++) {
				long double x = ct_in(ct_pts[i][0]), y = ct_in(ct_pts[i][1]), rx, ry;

				w_clog(ty, x, y, &rx, &ry);
				pc_check(!isnan(rx) && !isnan(ry), "clog [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), not a number",
					ct_tyname[ty], x, y, rx, ry);
			}
	}
	pc_pass();
}
