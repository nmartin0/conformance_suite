/*
 * iswalnum: alnum is alpha or digit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalnum.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i, nz = 0;
	wint_t w;
	pc_begin("XSH.iswalnum.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		nz += WC_NZ(iswalnum);
		pc_check(WC_NZ(iswalnum) == (WC_NZ(iswalpha) || WC_NZ(iswdigit)), "relation failed at position %d: WC_NZ(iswalnum) == (WC_NZ(iswalpha) || WC_NZ(iswdigit))", i);
	}
	/* the relation is not met by a function that answers the same for every character: the class has members and non-members among the 128 */
	pc_check(nz > 0 && nz < 128, "iswalnum() gave the same answer for all 128 characters (%d non-zero)", nz);
	pc_pass();
}
