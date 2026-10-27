/*
 * strtod: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.011
 */
#include "fc.h"
#include <string.h>

static const char *const in[] = { "2.5", "0x1p1", "7.25" };

int main(void)
{
	int j, k, err;
	pc_begin("XSH.strtod.011");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++) {
			errno = EDOM;
			(void)fc_fns[j].f(in[k], NULL);
			err = errno;
			pc_check(err == EDOM, "%s(\"%s\") changed errno to %s", fc_fns[j].name, in[k], pc_errname(err));
		}
	pc_pass();
}
