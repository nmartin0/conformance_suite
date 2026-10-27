/*
 * iswprint: print is graph or space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswprint.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswprint.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(WC_NZ(iswprint) == (WC_NZ(iswgraph) || i == 32), "relation failed at position %d: WC_NZ(iswprint) == (WC_NZ(iswgraph) || i == 32)", i);
	}
	pc_pass();
}
