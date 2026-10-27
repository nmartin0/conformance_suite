/*
 * Without an installed locale uselocale(0) returns LC_GLOBAL_LOCALE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uselocale.001
 */
#include "lc.h"

int main(void)
{
	locale_t a, b;

	pc_begin("XSH.uselocale.001");
	lc_init();
	pc_test();
	a = uselocale((locale_t)0);
	b = uselocale((locale_t)0);
	pc_check(a == LC_GLOBAL_LOCALE, "uselocale(0) in a thread with no locale installed did not return LC_GLOBAL_LOCALE");
	pc_check(b == LC_GLOBAL_LOCALE, "a second uselocale(0) did not return LC_GLOBAL_LOCALE");
	lc_check_items((locale_t)0, "after querying");
	pc_pass();
}
