/*
 * iswcntrl: cntrl not print.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswcntrl.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswcntrl.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		w = wc_ch[i];
		pc_check(!(WC_NZ(iswcntrl) && WC_NZ(iswprint)), "relation failed at position %d: !(WC_NZ(iswcntrl) && WC_NZ(iswprint))", i);
	}
	pc_pass();
}
