/*
 * iswpunct: punct with alnum is graph.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswpunct.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswpunct.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(WC_NZ(iswgraph) == (WC_NZ(iswalnum) || WC_NZ(iswpunct)), "relation failed at position %d: WC_NZ(iswgraph) == (WC_NZ(iswalnum) || WC_NZ(iswpunct))", i);
		pc_check(!(WC_NZ(iswpunct) && WC_NZ(iswalnum)), "relation failed at position %d: !(WC_NZ(iswpunct) && WC_NZ(iswalnum))", i);
	}
	pc_pass();
}
