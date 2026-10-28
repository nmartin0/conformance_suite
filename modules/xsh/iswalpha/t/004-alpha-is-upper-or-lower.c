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
	int i, nz = 0;
	wint_t w;
	pc_begin("XSH.iswalpha.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		nz += WC_NZ(iswalpha);
		pc_check(WC_NZ(iswalpha) == (WC_NZ(iswupper) || WC_NZ(iswlower)), "relation failed at position %d: WC_NZ(iswalpha) == (WC_NZ(iswupper) || WC_NZ(iswlower))", i);
	}
	/* the relation is not met by a function that answers the same for every character: the class has members and non-members among the 128 */
	pc_check(nz > 0 && nz < 128, "iswalpha() gave the same answer for all 128 characters (%d non-zero)", nz);
	pc_pass();
}
