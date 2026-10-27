/*
 * strtod: sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.003
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ "+2.5", 2.5L, -1 },
	{ "-2.5", -2.5L, -1 },
	{ "-0x1p1", -2.0L, -1 },
	{ "+0x1p1", 2.0L, -1 },
};

static const char *const nz[2] = { "-0", "-0.0" };

int main(void)
{
	int j, k;
	long double r;
	pc_begin("XSH.strtod.003");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	for (j = 0; j < 3; j++)
		for (k = 0; k < 2; k++) {
			r = fc_fns[j].f(nz[k], NULL);
			pc_check(r == 0.0L && signbit(r), "%s(\"%s\") did not return a negative zero", fc_fns[j].name, nz[k]);
		}
	pc_pass();
}
