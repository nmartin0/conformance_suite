/*
 * wctrans: handle usable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctrans.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	wctrans_t up, lo;
	pc_begin("XSH.wctrans.003");
	up = wctrans("toupper");
	lo = wctrans("tolower");
	pc_check(up != (wctrans_t)0 && lo != (wctrans_t)0, "wctrans() returned 0 for a standard name");
	pc_test();
	pc_check(towctrans(L'a', up) == (wint_t)L'A', "towctrans(a) with the toupper handle did not give A");
	pc_check(towctrans(L'A', lo) == (wint_t)L'a', "towctrans(A) with the tolower handle did not give a");
	pc_pass();
}
