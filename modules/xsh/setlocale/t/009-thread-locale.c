/*
 * A thread-local locale shields the thread from setlocale().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.009
 */
#include "lc.h"

int main(void)
{
	locale_t obj;

	pc_begin("XSH.setlocale.009");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	if (uselocale(obj) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "the thread-local POSIX object was affected by setlocale(): code set \"%s\"", nl_langinfo(CODESET));
	if (uselocale(LC_GLOBAL_LOCALE) == (locale_t)0)
		pc_unresolved("uselocale(LC_GLOBAL_LOCALE) failed");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after uselocale(LC_GLOBAL_LOCALE) the code set is \"%s\", not the global locale's", nl_langinfo(CODESET));
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	pc_pass();
}
