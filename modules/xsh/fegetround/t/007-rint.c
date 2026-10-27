/*
 * rint() follows the direction set by fesetround().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.007
 */
#include "fh.h"

/* Expected rint() of the arguments 2.25, 2.75, 2.5, -2.5 under each direction;
 * under to-nearest only the first nk are judged (the others are halfway cases). */
struct row { int mode; int nk; double want[4]; };

static const struct row rows[] = {
#ifdef FE_TONEAREST
	{ FE_TONEAREST, 2, { 2.0, 3.0, 0.0, 0.0 } },
#endif
#ifdef FE_UPWARD
	{ FE_UPWARD, 4, { 3.0, 3.0, 3.0, -2.0 } },
#endif
#ifdef FE_DOWNWARD
	{ FE_DOWNWARD, 4, { 2.0, 2.0, 2.0, -3.0 } },
#endif
#ifdef FE_TOWARDZERO
	{ FE_TOWARDZERO, 4, { 2.0, 2.0, 2.0, -2.0 } },
#endif
	{ -1, 0, { 0.0, 0.0, 0.0, 0.0 } }
};

int main(void)
{
	static const double in[4] = { 2.25, 2.75, 2.5, -2.5 };
	volatile double x, r;
	int i, k, m0, n = (int)(sizeof rows / sizeof rows[0]) - 1;

	pc_begin("XSH.fegetround.007");
	if (n == 0)
		pc_notinuse("no rounding direction macro is defined");
	m0 = fegetround();
	pc_test();
	for (i = 0; i < n; i++) {
		if (fesetround(rows[i].mode) != 0)
			pc_unresolved("fesetround(%d) failed", rows[i].mode);
		for (k = 0; k < rows[i].nk; k++) {
			x = in[k];
			r = rint(x);
			pc_check(r == rows[i].want[k], "rint(%g) returned %g under direction %d, expected %g", (double)x, (double)r, rows[i].mode, rows[i].want[k]);
		}
	}
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_pass();
}
