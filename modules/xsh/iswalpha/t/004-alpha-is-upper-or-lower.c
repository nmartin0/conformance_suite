/*
 * iswalpha: alpha is upper or lower.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalpha.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswalpha.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(WC_NZ(iswalpha) == (WC_NZ(iswupper) || WC_NZ(iswlower)), "relation failed at position %d: WC_NZ(iswalpha) == (WC_NZ(iswupper) || WC_NZ(iswlower))", i);
	}
	pc_pass();
}
