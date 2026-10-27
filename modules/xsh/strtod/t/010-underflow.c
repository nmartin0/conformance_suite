/*
 * strtod: underflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.010
 */
#include "fc.h"
#include <string.h>

static const char *const in[] = { "1e-100000", "-1e-100000", "0x1p-100000" };

int main(void)
{
	int j, k, err;
	char *e;
	long double r;
	pc_begin("XSH.strtod.010");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(in[k], &e);
			err = errno;
			pc_check(fabsl(r) <= fc_fns[j].minnorm, "%s(\"%s\") returned a magnitude above the smallest normalized number", fc_fns[j].name, in[k]);
			pc_check(err == ERANGE, "%s(\"%s\") set errno to %s, expected ERANGE", fc_fns[j].name, in[k], pc_errname(err));
			pc_check(e == in[k] + strlen(in[k]), "%s(\"%s\") left the end pointer at offset %ld", fc_fns[j].name, in[k], (long)(e - in[k]));
		}
	pc_pass();
}
