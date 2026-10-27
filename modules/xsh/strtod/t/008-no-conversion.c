/*
 * strtod: no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.008
 */
#include "fc.h"
#include <string.h>

static const char *const bad[] = { "", "   ", "abc", "+", "-", ".", "e5", "+.", ",5" };

int main(void)
{
	int j, k, err;
	char *e;
	long double r;
	pc_begin("XSH.strtod.008");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof bad / sizeof bad[0]); k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(bad[k], &e);
			err = errno;
			pc_check(r == 0.0L, "%s(\"%s\") returned non-zero", fc_fns[j].name, bad[k]);
			pc_check(e == bad[k], "%s(\"%s\") did not store the start of the string in the end pointer", fc_fns[j].name, bad[k]);
			pc_check(err == 0 || err == EINVAL, "%s(\"%s\") set errno to %s", fc_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
