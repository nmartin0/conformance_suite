/*
 * wcstod: no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.008
 */
#include "fc.h"
#include <string.h>

static const wchar_t *const bad[] = { L"", L"   ", L"abc", L"+", L"-", L".", L"e5", L"+.", L",5" };

int main(void)
{
	int j, k, err;
	wchar_t *e;
	long double r;
	pc_begin("XSH.wcstod.008");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof bad / sizeof bad[0]); k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(bad[k], &e);
			err = errno;
			pc_check(r == 0.0L, "%s(\"%ls\") returned non-zero", fc_fns[j].name, bad[k]);
			pc_check(e == bad[k], "%s(\"%ls\") did not store the start of the string in the end pointer", fc_fns[j].name, bad[k]);
			pc_check(err == 0 || err == EINVAL, "%s(\"%ls\") set errno to %s", fc_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
