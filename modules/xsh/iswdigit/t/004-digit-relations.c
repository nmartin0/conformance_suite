/*
 * iswdigit: digit relations.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswdigit.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswdigit.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_DIGIT, i))
			continue;
		w = wc_ch[i];
		pc_check(WC_NZ(iswxdigit), "relation failed at position %d: WC_NZ(iswxdigit)", i);
		pc_check(WC_NZ(iswalnum), "relation failed at position %d: WC_NZ(iswalnum)", i);
		pc_check(!WC_NZ(iswalpha), "relation failed at position %d: !WC_NZ(iswalpha)", i);
	}
	pc_pass();
}
