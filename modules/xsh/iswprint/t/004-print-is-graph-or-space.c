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
	int i, nz = 0;
	wint_t w;
	pc_begin("XSH.iswprint.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		nz += WC_NZ(iswprint);
		pc_check(WC_NZ(iswprint) == (WC_NZ(iswgraph) || i == 32), "relation failed at position %d: WC_NZ(iswprint) == (WC_NZ(iswgraph) || i == 32)", i);
	}
	/* the relation is not met by a function that answers the same for every character: the class has members and non-members among the 128 */
	pc_check(nz > 0 && nz < 128, "iswprint() gave the same answer for all 128 characters (%d non-zero)", nz);
	pc_pass();
}
