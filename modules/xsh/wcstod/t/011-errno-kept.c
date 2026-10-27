/*
 * wcstod: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.011
 */
#include "fc.h"
#include <string.h>

static const wchar_t *const in[] = { L"2.5", L"0x1p1", L"7.25" };

int main(void)
{
	int j, k, err;
	pc_begin("XSH.wcstod.011");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++) {
			errno = EDOM;
			(void)fc_fns[j].f(in[k], NULL);
			err = errno;
			pc_check(err == EDOM, "%s(\"%ls\") changed errno to %s", fc_fns[j].name, in[k], pc_errname(err));
		}
	pc_pass();
}
