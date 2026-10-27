/*
 * wcstoimax: no conversion u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstoimax.020
 */
#define IC_UMAX
#include "ic.h"
#include <string.h>

static const wchar_t *const bad[] = { L"", L"   ", L"xyz", L"+", L"-", L"+ 1" };

int main(void)
{
	int j, k, err;
	wchar_t *e;
	R r;
	pc_begin("XSH.wcstoimax.020");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < (int)(sizeof bad / sizeof bad[0]); k++) {
			e = NULL;
			errno = 0;
			r = ic_fns[j].f(bad[k], &e, 10);
			err = errno;
			pc_check(r == 0, "%s(\"%ls\") returned non-zero", ic_fns[j].name, bad[k]);
			pc_check(e == bad[k], "%s(\"%ls\") did not store the start of the string in the end pointer", ic_fns[j].name, bad[k]);
			pc_check(err == 0 || err == EINVAL, "%s(\"%ls\") set errno to %s", ic_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
