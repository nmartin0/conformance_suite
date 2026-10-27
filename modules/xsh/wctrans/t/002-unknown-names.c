/*
 * wctrans: unknown names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctrans.002
 */
#include "wc.h"
#include <string.h>

static const char *const bad[4] = { "", "no such map", "toupper ", "to upper" };

int main(void)
{
	int k, e;
	wctrans_t h;
	pc_begin("XSH.wctrans.002");
	pc_test();
	for (k = 0; k < 4; k++) {
		errno = 0;
		h = wctrans(bad[k]);
		e = errno;
		pc_check(h == (wctrans_t)0, "wctrans(\"%s\") returned non-zero", bad[k]);
		pc_check(e == 0 || e == EINVAL, "wctrans(\"%s\") set errno to %s", bad[k], pc_errname(e));
	}
	pc_pass();
}
