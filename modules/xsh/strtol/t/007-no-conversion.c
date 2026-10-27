/*
 * strtol: no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.007
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const char *const bad[] = { "", "   ", "xyz", "+", "-", "+ 1" };

int main(void)
{
	int j, k, err;
	char *e;
	R r;
	pc_begin("XSH.strtol.007");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < (int)(sizeof bad / sizeof bad[0]); k++) {
			e = NULL;
			errno = 0;
			r = ic_fns[j].f(bad[k], &e, 10);
			err = errno;
			pc_check(r == 0, "%s(\"%s\") returned non-zero", ic_fns[j].name, bad[k]);
			pc_check(e == bad[k], "%s(\"%s\") did not store the start of the string in the end pointer", ic_fns[j].name, bad[k]);
			pc_check(err == 0 || err == EINVAL, "%s(\"%s\") set errno to %s", ic_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
