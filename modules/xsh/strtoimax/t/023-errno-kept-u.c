/*
 * strtoimax: errno kept u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.023
 */
#define IC_UMAX
#include "ic.h"
#include <string.h>

static const struct { const char *in; int base; } cs[] = { { "12", 10 }, { "0x1f", 16 }, { "077", 0 } };

int main(void)
{
	int j, k, err;
	pc_begin("XSH.strtoimax.023");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < 3; k++) {
			errno = EDOM;
			(void)ic_fns[j].f(cs[k].in, NULL, cs[k].base);
			err = errno;
			pc_check(err == EDOM, "%s(\"%s\") changed errno to %s", ic_fns[j].name, cs[k].in, pc_errname(err));
		}
	pc_pass();
}
