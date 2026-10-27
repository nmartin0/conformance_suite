/*
 * isalpha: the _l form with a locale object for the POSIX locale
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isalpha.004
 */
#include "ct.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.isalpha.004");
	ct_init();
	loc = ct_cloc();
	pc_test();
	ct_scan(C_ALPHA, 1, 2, 3, loc);
	ct_scan(C_ALPHA, 0, 2, 3, loc);
	pc_pass();
}
