/*
 * imaxdiv: truncation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxdiv.001
 */
#include "pc.h"
#include <inttypes.h>
static imaxdiv_t (*volatile f_imaxdiv)(intmax_t, intmax_t) = imaxdiv;

int main(void)
{
	static const struct { intmax_t n, d, q, r; } t[] = {
		{ 7, 2, 3, 1 }, { -7, 2, -3, -1 }, { 7, -2, -3, 1 }, { -7, -2, 3, -1 },
		{ 1, 5, 0, 1 }, { -1, 5, 0, -1 }, { 6, 3, 2, 0 }, { 0, 5, 0, 0 }, { 5, 1, 5, 0 }, { 5, -1, -5, 0 }
	};
	size_t k;
	imaxdiv_t r;
	pc_begin("XSH.imaxdiv.001");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++) {
		r = f_imaxdiv(t[k].n, t[k].d);
		pc_check(r.quot == t[k].q, "imaxdiv(%jd, %jd).quot was %jd, expected %jd", t[k].n, t[k].d, r.quot, t[k].q);
		pc_check(r.rem == t[k].r, "imaxdiv(%jd, %jd).rem was %jd, expected %jd", t[k].n, t[k].d, r.rem, t[k].r);
	}
	pc_pass();
}
