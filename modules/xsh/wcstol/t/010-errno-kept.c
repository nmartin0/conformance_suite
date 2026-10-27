/*
 * wcstol: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.010
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const struct { const wchar_t *in; int base; } cs[] = { { L"12", 10 }, { L"0x1f", 16 }, { L"077", 0 } };

int main(void)
{
	int j, k, err;
	pc_begin("XSH.wcstol.010");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < 3; k++) {
			errno = EDOM;
			(void)ic_fns[j].f(cs[k].in, NULL, cs[k].base);
			err = errno;
			pc_check(err == EDOM, "%s(\"%ls\") changed errno to %s", ic_fns[j].name, cs[k].in, pc_errname(err));
		}
	pc_pass();
}
