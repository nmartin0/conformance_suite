/*
 * towctrans: toupper.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towctrans.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wctrans_t up, lo;
	wint_t want;
	pc_begin("XSH.towctrans.001");
	wc_init();
	up = wctrans("toupper");
	lo = wctrans("tolower");
	pc_check(up != (wctrans_t)0 && lo != (wctrans_t)0, "wctrans() returned 0 for a standard name");
	pc_test();
	for (i = 0; i < 128; i++) {
		want = wc_in(C_LOWER, i) ? wc_ch[i - 32] : wc_ch[i];
		pc_check(towctrans(wc_ch[i], up) == want, "towctrans() with the toupper handle returned the wrong value at position %d", i);
		pc_check(towctrans(wc_ch[i], up) == towupper(wc_ch[i]), "towctrans() and towupper() differ at position %d", i);
	}
	pc_pass();
}
