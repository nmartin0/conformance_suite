/*
 * ccosh of -z returns the same value as ccosh of z, part for part (equal
 * values, zero signs included), at ordinary points, at real-axis points
 * where branch cuts lie, and at points with infinite parts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ccosh.013
 */
#include "mt.h"

CT_WRAP_CC(ccosh)

int main(void)
{
	pc_begin("XSH.ccosh.013");
	MT_NEED_MXC();
	pc_test();
	ct_sym("ccosh", w_ccosh, SYM_EVEN);
	/* A function with no value anywhere keeps any symmetry trivially: at ordinary points the result is a number. */
	{
		int ty, i;

		for (ty = 0; ty < 3; ty++)
			for (i = 0; i < 3; i++) {
				long double x = ct_in(ct_pts[i][0]), y = ct_in(ct_pts[i][1]), rx, ry;

				w_ccosh(ty, x, y, &rx, &ry);
				pc_check(!isnan(rx) && !isnan(ry), "ccosh [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), not a number",
					ct_tyname[ty], x, y, rx, ry);
			}
	}
	pc_pass();
}
