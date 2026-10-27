/*
 * Copies of the POSIX global locale, also through uselocale(0).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.duplocale.005
 */
#include "lc.h"

int main(void)
{
	locale_t d1, d2;

	pc_begin("XSH.duplocale.005");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	d1 = duplocale(LC_GLOBAL_LOCALE);
	pc_check(d1 != (locale_t)0, "duplocale(LC_GLOBAL_LOCALE) failed, errno %s", pc_errname(errno));
	d2 = duplocale(uselocale((locale_t)0));
	pc_check(d2 != (locale_t)0, "duplocale(uselocale(0)) failed, errno %s", pc_errname(errno));
	lc_check_items(d1, "copy of the global locale");
	lc_check_items(d2, "copy through uselocale(0)");
	pc_check(uselocale(d1) == LC_GLOBAL_LOCALE, "uselocale() did not accept the copy, or did not return LC_GLOBAL_LOCALE");
	lc_check_items((locale_t)0, "the copy installed");
	(void)uselocale(LC_GLOBAL_LOCALE);
	freelocale(d1);
	freelocale(d2);
	pc_pass();
}
