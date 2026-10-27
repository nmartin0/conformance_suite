/*
 * imaxdiv: identity.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxdiv.002
 */
#include "pc.h"
#include <inttypes.h>
static imaxdiv_t (*volatile f_imaxdiv)(intmax_t, intmax_t) = imaxdiv;

int main(void)
{
	static const intmax_t v[] = { 0, 1, 2, 3, 7, 10, 99, 1000, 65537, 2147483647, 5000000000LL };
	size_t i, j;
	int si, sj;
	pc_begin("XSH.imaxdiv.002");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++)
		for (j = 1; j < sizeof v / sizeof v[0]; j++)
			for (si = -1; si <= 1; si += 2)
				for (sj = -1; sj <= 1; sj += 2) {
					intmax_t n = si * v[i], d = sj * v[j];
					imaxdiv_t r = f_imaxdiv(n, d);
					pc_check(r.quot * d + r.rem == n, "imaxdiv(%jd, %jd): quot * denom + rem is not numer", n, d);
					pc_check((r.rem < 0 ? -r.rem : r.rem) < (d < 0 ? -d : d), "imaxdiv(%jd, %jd): the remainder is too large", n, d);
					pc_check(r.rem == 0 || (r.rem < 0) == (n < 0), "imaxdiv(%jd, %jd): the remainder has the wrong sign", n, d);
				}
	pc_pass();
}
