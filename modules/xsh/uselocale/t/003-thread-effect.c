/*
 * A thread-local locale changes the thread and not the global locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uselocale.003
 */
#include "lc.h"

int main(void)
{
	char before[512], after[512];
	locale_t objl;

	pc_begin("XSH.uselocale.003");
	lc_init();
	lc_need();
	objl = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (objl == (locale_t)0)
		pc_unresolved("newlocale() could not make an object from the second locale");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	lc_q(LC_ALL, before, sizeof before);
	pc_test();
	if (uselocale(objl) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "with the second locale installed the code set is \"%s\"", nl_langinfo(CODESET));
	lc_q(LC_ALL, after, sizeof after);
	pc_check(strcmp(before, after) == 0, "installing a thread-local locale changed the global locale from \"%s\" to \"%s\"", before, after);
	(void)uselocale(LC_GLOBAL_LOCALE);
	pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "after uselocale(LC_GLOBAL_LOCALE) the code set is \"%s\", expected the POSIX one", nl_langinfo(CODESET));
	freelocale(objl);
	pc_pass();
}
