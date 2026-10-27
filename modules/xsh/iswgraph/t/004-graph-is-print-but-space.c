/*
 * iswgraph: graph is print but space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswgraph.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswgraph.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(WC_NZ(iswgraph) == (WC_NZ(iswprint) && i != 32), "relation failed at position %d: WC_NZ(iswgraph) == (WC_NZ(iswprint) && i != 32)", i);
		pc_check(WC_NZ(iswgraph) == (WC_NZ(iswalnum) || WC_NZ(iswpunct)), "relation failed at position %d: WC_NZ(iswgraph) == (WC_NZ(iswalnum) || WC_NZ(iswpunct))", i);
	}
	pc_pass();
}
