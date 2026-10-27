/*
 * duplocale(LC_GLOBAL_LOCALE) is a snapshot.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.duplocale.004
 */
#include "lc.h"

int main(void)
{
	locale_t d;

	pc_begin("XSH.duplocale.004");
	lc_init();
	lc_need();
	if (setlocale(LC_ALL, "C") == NULL || setlocale(LC_CTYPE, lc_loc) == NULL)
		pc_unresolved("could not set the global locale up");
	pc_test();
	d = duplocale(LC_GLOBAL_LOCALE);
	pc_check(d != (locale_t)0, "duplocale(LC_GLOBAL_LOCALE) failed, errno %s", pc_errname(errno));
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(strcmp(nl_langinfo_l(CODESET, d), "UTF-8") == 0, "the copy of the global locale has code set \"%s\", expected the one copied", nl_langinfo_l(CODESET, d));
	lc_check_cat(d, LC_TIME, "the copy of the global locale");
	lc_check_cat(d, LC_NUMERIC, "the copy of the global locale");
	freelocale(d);
	pc_pass();
}
