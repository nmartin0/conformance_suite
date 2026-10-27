/*
 * iswblank: blank is space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswblank.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswblank.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_BLANK, i))
			continue;
		w = wc_ch[i];
		pc_check(WC_NZ(iswspace), "relation failed at position %d: WC_NZ(iswspace)", i);
	}
	pc_pass();
}
