/*
 * wcstod: overflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.009
 */
#include "fc.h"
#include <string.h>

static const wchar_t *const in[] = { L"1e100000", L"-1e100000", L"0x1p100000" };

int main(void)
{
	int j, k, err, neg;
	wchar_t *e;
	long double r;
	pc_begin("XSH.wcstod.009");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++) {
			neg = in[k][0] == L'-';
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(in[k], &e);
			err = errno;
			pc_check(r == (neg ? -fc_fns[j].huge : fc_fns[j].huge), "%s(\"%ls\") did not return the huge value", fc_fns[j].name, in[k]);
			pc_check(err == ERANGE, "%s(\"%ls\") set errno to %s, expected ERANGE", fc_fns[j].name, in[k], pc_errname(err));
			pc_check(e == in[k] + wcslen(in[k]), "%s(\"%ls\") left the end pointer at offset %ld", fc_fns[j].name, in[k], (long)(e - in[k]));
		}
	pc_pass();
}
