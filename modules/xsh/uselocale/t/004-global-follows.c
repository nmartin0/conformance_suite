/*
 * After uninstalling, the thread follows the global locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uselocale.004
 */
#include "lc.h"

int main(void)
{
	locale_t obj;

	pc_begin("XSH.uselocale.004");
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
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "with the POSIX object installed the code set is \"%s\"", nl_langinfo(CODESET));
	(void)uselocale(LC_GLOBAL_LOCALE);
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after uninstalling the code set is \"%s\", expected the global locale's", nl_langinfo(CODESET));
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "after setting the global locale to C the code set is \"%s\", expected the POSIX one", nl_langinfo(CODESET));
	freelocale(obj);
	pc_pass();
}
