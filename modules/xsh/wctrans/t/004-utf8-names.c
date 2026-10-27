/*
 * wctrans: utf8 names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctrans.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.wctrans.004");
	wc_utf8();
	pc_test();
	pc_check(wctrans("tolower") != (wctrans_t)0, "wctrans(\"tolower\") returned 0");
	pc_check(wctrans("toupper") != (wctrans_t)0, "wctrans(\"toupper\") returned 0");
	pc_pass();
}
