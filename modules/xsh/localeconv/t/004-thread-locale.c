/*
 * localeconv() follows the thread's current locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.004
 */
#include "lconv.h"

int main(void)
{
	locale_t obj;

	pc_begin("XSH.localeconv.004");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	pc_test();
	if (uselocale(obj) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	lcv_numeric(localeconv(), "thread-local POSIX object");
	lcv_mstr(localeconv(), "thread-local POSIX object");
	lcv_mchar(localeconv(), "thread-local POSIX object");
	(void)uselocale(LC_GLOBAL_LOCALE);
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	pc_pass();
}
