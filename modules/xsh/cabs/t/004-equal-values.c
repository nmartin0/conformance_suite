/*
 * cabs of x + iy, of y + ix and of x - iy are the same value, at ordinary
 * points and at points with an infinite or NaN part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cabs.004
 */
#include "mt.h"

CT_WRAP_CR(cabs)

static const struct ex pts[][2] = {
	{ X_V(3.0L), X_V(4.0L) },
	{ X_V(-1.5L), X_V(2.0L) },
	{ X_V(0.5L), X_V(-0.25L) },
	{ X_V(1.0L), X_V(2.5L) },
	{ X_P0, X_V(3.0L) },
	{ X_V(-2.0L), X_P0 },
	{ X_PINF, X_V(1.0L) },
	{ X_NINF, X_V(-1.0L) },
	{ X_PINF, X_NAN },
	{ X_NAN, X_NINF },
	{ X_NAN, X_V(2.0L) },
	{ X_V(2.0L), X_NAN },
	{ X_NAN, X_NAN },
	{ X_NINF, X_PINF },
};

int main(void)
{
	int ty, i;

	pc_begin("XSH.cabs.004");
	MT_NEED_MXC();
	pc_test();
	for (ty = 0; ty < 3; ty++)
		for (i = 0; i < (int)(sizeof pts / sizeof pts[0]); i++) {
			long double x, y, a, b, c, u;

			if (!ct_usable(pts[i][0]) || !ct_usable(pts[i][1]))
				continue;
			x = ct_in(pts[i][0]);
			y = ct_in(pts[i][1]);
			w_cabs(ty, x, y, &a, &u);
			w_cabs(ty, y, x, &b, &u);
			w_cabs(ty, x, -y, &c, &u);
			if (i < 4)
				pc_check(!isnan(a), "cabs [%s]: (%Lg, %Lg) at an ordinary point gives a NaN", ct_tyname[ty], x, y);
			pc_check(ct_same(a, b) && ct_same(a, c), "cabs [%s]: (%Lg, %Lg) gives %Lg, swapped parts give %Lg, negated imaginary part gives %Lg",
				ct_tyname[ty], x, y, a, b, c);
		}
	pc_pass();
}
