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
	int i;
	wint_t w;
	pc_begin("XSH.iswalnum.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(WC_NZ(iswalnum) == (WC_NZ(iswalpha) || WC_NZ(iswdigit)), "relation failed at position %d: WC_NZ(iswalnum) == (WC_NZ(iswalpha) || WC_NZ(iswdigit))", i);
	}
	pc_pass();
}
