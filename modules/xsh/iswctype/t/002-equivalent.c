/*
 * iswctype: equivalent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswctype.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int k, i;
	wctype_t h[C_N];
	pc_begin("XSH.iswctype.002");
	wc_init();
	for (k = 0; k < C_N; k++) {
		h[k] = wctype(wc_cname[k]);
		pc_check(h[k] != (wctype_t)0, "wctype(\"%s\") returned 0", wc_cname[k]);
	}
	pc_test();
	for (k = 0; k < C_N; k++)
		for (i = 0; i < 128; i++)
			pc_check((iswctype(wc_ch[i], h[k]) != 0) == (wc_isw[k](wc_ch[i]) != 0), "iswctype() and isw%s() differ at position %d", wc_cname[k], i);
	pc_pass();
}
