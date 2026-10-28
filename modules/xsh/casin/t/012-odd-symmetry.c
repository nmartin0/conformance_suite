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
	/* A function with no value anywhere keeps any symmetry trivially: at ordinary points the result is a number. */
	{
		int ty, i;

		for (ty = 0; ty < 3; ty++)
			for (i = 0; i < 3; i++) {
				long double x = ct_in(ct_pts[i][0]), y = ct_in(ct_pts[i][1]), rx, ry;

				w_casin(ty, x, y, &rx, &ry);
				pc_check(!isnan(rx) && !isnan(ry), "casin [%s]: at (%Lg, %Lg) the result is (%Lg, %Lg), not a number",
					ct_tyname[ty], x, y, rx, ry);
			}
	}
	pc_pass();
}
