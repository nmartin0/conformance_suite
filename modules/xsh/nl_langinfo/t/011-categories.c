/*
 * Items follow the category <langinfo.h> assigns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.011
 */
#include "lc.h"

int main(void)
{
	locale_t obj;
	int c;

	pc_begin("XSH.nl_langinfo.011");
	lc_init();
	lc_need();
	pc_test();
	for (c = 0; c < 6; c++) {
		if (setlocale(LC_ALL, "C") == NULL || setlocale(lc_cat[c], lc_loc) == NULL)
			pc_unresolved("could not set category %d to the second locale", lc_cat[c]);
		obj = newlocale(lc_mask[c], lc_loc, (locale_t)0);
		if (obj == (locale_t)0)
			pc_unresolved("newlocale() could not make an object for category %d", lc_cat[c]);
		if (lc_cat[c] == LC_CTYPE) {
			pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "LC_CTYPE set: the code set is \"%s\"", nl_langinfo(CODESET));
			pc_check(strcmp(nl_langinfo_l(CODESET, obj), "UTF-8") == 0, "LC_CTYPE_MASK object: the code set is \"%s\"", nl_langinfo_l(CODESET, obj));
			lc_check_cat((locale_t)0, LC_TIME, "LC_CTYPE set");
			lc_check_cat((locale_t)0, LC_NUMERIC, "LC_CTYPE set");
			lc_check_cat((locale_t)0, LC_MESSAGES, "LC_CTYPE set");
			lc_check_cat(obj, LC_TIME, "LC_CTYPE_MASK object");
			lc_check_cat(obj, LC_NUMERIC, "LC_CTYPE_MASK object");
			lc_check_cat(obj, LC_MESSAGES, "LC_CTYPE_MASK object");
		} else {
			pc_check(strcmp(nl_langinfo(CODESET), lc_c0) == 0, "category %d set: the code set is \"%s\", expected the POSIX one", lc_cat[c], nl_langinfo(CODESET));
			pc_check(strcmp(nl_langinfo_l(CODESET, obj), lc_c0) == 0, "object for category %d: the code set is \"%s\", expected the POSIX one", lc_cat[c], nl_langinfo_l(CODESET, obj));
		}
		freelocale(obj);
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
