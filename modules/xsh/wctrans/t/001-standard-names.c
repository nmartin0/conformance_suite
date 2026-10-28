/*
 * wctrans: standard names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctrans.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.wctrans.001");
	pc_test();
	pc_check(wctrans("tolower") != (wctrans_t)0, "wctrans(\"tolower\") returned 0");
	pc_check(wctrans("toupper") != (wctrans_t)0, "wctrans(\"toupper\") returned 0");
	/* the handles are usable: they map a letter of the POSIX locale */
	pc_check(towctrans(L'a', wctrans("toupper")) == L'A', "the toupper handle did not map L'a' to L'A'");
	pc_check(towctrans(L'A', wctrans("tolower")) == L'a', "the tolower handle did not map L'A' to L'a'");
	pc_pass();
}
