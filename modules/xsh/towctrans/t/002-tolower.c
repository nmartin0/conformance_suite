/*
 * towctrans: tolower.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towctrans.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wctrans_t up, lo;
	wint_t want;
	pc_begin("XSH.towctrans.002");
	wc_init();
	up = wctrans("toupper");
	lo = wctrans("tolower");
	pc_check(up != (wctrans_t)0 && lo != (wctrans_t)0, "wctrans() returned 0 for a standard name");
	pc_test();
	for (i = 0; i < 128; i++) {
		want = wc_in(C_UPPER, i) ? wc_ch[i + 32] : wc_ch[i];
		pc_check(towctrans(wc_ch[i], lo) == want, "towctrans() with the tolower handle returned the wrong value at position %d", i);
		pc_check(towctrans(wc_ch[i], lo) == towlower(wc_ch[i]), "towctrans() and towlower() differ at position %d", i);
	}
	pc_pass();
}
