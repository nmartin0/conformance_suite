/*
 * wcstod: underflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.010
 */
#include "fc.h"
#include <string.h>

static const wchar_t *const in[] = { L"1e-100000", L"-1e-100000", L"0x1p-100000" };

int main(void)
{
	int j, k, err;
	wchar_t *e;
	long double r;
	pc_begin("XSH.wcstod.010");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(in[k], &e);
			err = errno;
			pc_check(fabsl(r) <= fc_fns[j].minnorm, "%s(\"%ls\") returned a magnitude above the smallest normalized number", fc_fns[j].name, in[k]);
			pc_check(err == ERANGE, "%s(\"%ls\") set errno to %s, expected ERANGE", fc_fns[j].name, in[k], pc_errname(err));
			pc_check(e == in[k] + wcslen(in[k]), "%s(\"%ls\") left the end pointer at offset %ld", fc_fns[j].name, in[k], (long)(e - in[k]));
		}
	pc_pass();
}
