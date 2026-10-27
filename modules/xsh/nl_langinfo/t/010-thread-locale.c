/*
 * nl_langinfo() reads the thread's current locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.010
 */
#include "lc.h"

int main(void)
{
	locale_t obj, objl;

	pc_begin("XSH.nl_langinfo.010");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	objl = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (obj == (locale_t)0 || objl == (locale_t)0)
		pc_unresolved("newlocale() could not make the locale objects");
	pc_test();
	if (uselocale(objl) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "with the second locale installed the code set is \"%s\"", nl_langinfo(CODESET));
	(void)uselocale(LC_GLOBAL_LOCALE);
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "after uselocale(LC_GLOBAL_LOCALE) the code set is \"%s\", expected the POSIX one", nl_langinfo(CODESET));
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	(void)uselocale(obj);
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "with the POSIX object installed the code set is \"%s\"", nl_langinfo(CODESET));
	(void)uselocale(LC_GLOBAL_LOCALE);
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after uselocale(LC_GLOBAL_LOCALE) the code set is \"%s\", expected the global locale's", nl_langinfo(CODESET));
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	freelocale(objl);
	pc_pass();
}
