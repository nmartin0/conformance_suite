/*
 * iswctype: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswctype.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int k;
	wctype_t h[C_N];
	pc_begin("XSH.iswctype.004");
	wc_init();
	for (k = 0; k < C_N; k++) {
		h[k] = wctype(wc_cname[k]);
		pc_check(h[k] != (wctype_t)0, "wctype(\"%s\") returned 0", wc_cname[k]);
	}
	pc_test();
	for (k = 0; k < C_N; k++)
		pc_check(iswctype(WEOF, h[k]) == 0, "iswctype(WEOF) with the %s handle returned non-zero", wc_cname[k]);
	pc_pass();
}
