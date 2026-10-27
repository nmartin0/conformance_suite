/*
 * nl_langinfo_l() does not read the global or the thread's locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.009
 */
#include "lc.h"

int main(void)
{
	locale_t obj, objl;

	pc_begin("XSH.nl_langinfo.009");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	objl = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (obj == (locale_t)0 || objl == (locale_t)0)
		pc_unresolved("newlocale() could not make the locale objects");
	pc_test();
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	lc_check_items(obj, "object for C, global locale changed");
	if (uselocale(objl) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	lc_check_items(obj, "object for C, thread locale changed");
	pc_check(strcmp(nl_langinfo_l(CODESET, objl), "UTF-8") == 0, "the object for the second locale gives code set \"%s\"", nl_langinfo_l(CODESET, objl));
	(void)uselocale(LC_GLOBAL_LOCALE);
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	freelocale(objl);
	pc_pass();
}
