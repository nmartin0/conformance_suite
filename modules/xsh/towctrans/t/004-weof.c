/*
 * towctrans: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towctrans.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	wctrans_t up, lo;
	pc_begin("XSH.towctrans.004");
	wc_init();
	up = wctrans("toupper");
	lo = wctrans("tolower");
	pc_check(up != (wctrans_t)0 && lo != (wctrans_t)0, "wctrans() returned 0 for a standard name");
	pc_test();
	/* control: letters are converted with the handles, so WEOF coming back as WEOF is not the answer of a function that returns -1 */
	pc_check(towctrans(L'a', up) == L'A', "towctrans(L'a') with the toupper handle did not give L'A'");
	pc_check(towctrans(L'A', lo) == L'a', "towctrans(L'A') with the tolower handle did not give L'a'");
	pc_check(towctrans(WEOF, up) == WEOF, "towctrans(WEOF) with the toupper handle did not return WEOF");
	pc_check(towctrans(WEOF, lo) == WEOF, "towctrans(WEOF) with the tolower handle did not return WEOF");
	pc_pass();
}
