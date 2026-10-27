/*
 * Names taken from the global locale with LC_CTYPE from another locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlocalename_l.003
 */
#include "lc.h"

int main(void)
{
	char ct[512], tm[512], all[512];
	locale_t o;
	const char *p;

	pc_begin("XSH.getlocalename_l.003");
	lc_init();
	lc_need();
	if (setlocale(LC_ALL, "C") == NULL || setlocale(LC_CTYPE, lc_loc) == NULL)
		pc_unresolved("could not set the global locale up");
	pc_test();
	p = getlocalename_l(LC_CTYPE, LC_GLOBAL_LOCALE);
	pc_check(p != NULL, "getlocalename_l(LC_CTYPE, LC_GLOBAL_LOCALE) returned a null pointer");
	lc_cp(ct, sizeof ct, p);
	p = getlocalename_l(LC_TIME, LC_GLOBAL_LOCALE);
	pc_check(p != NULL, "getlocalename_l(LC_TIME, LC_GLOBAL_LOCALE) returned a null pointer");
	lc_cp(tm, sizeof tm, p);
	p = getlocalename_l(LC_ALL, LC_GLOBAL_LOCALE);
	pc_check(p != NULL, "getlocalename_l(LC_ALL, LC_GLOBAL_LOCALE) returned a null pointer");
	lc_cp(all, sizeof all, p);

	o = newlocale(LC_CTYPE_MASK, ct, (locale_t)0);
	pc_check(o != (locale_t)0, "newlocale() did not accept the LC_CTYPE name \"%s\", errno %s", ct, pc_errname(errno));
	pc_check(strcmp(nl_langinfo_l(CODESET, o), "UTF-8") == 0, "the object made from the LC_CTYPE name has code set \"%s\"", nl_langinfo_l(CODESET, o));
	freelocale(o);
	o = newlocale(LC_CTYPE_MASK, tm, (locale_t)0);
	pc_check(o != (locale_t)0, "newlocale() did not accept the LC_TIME name \"%s\", errno %s", tm, pc_errname(errno));
	pc_check(strcmp(nl_langinfo_l(CODESET, o), lc_c0) == 0, "the object made from the LC_TIME name has code set \"%s\", expected the POSIX one", nl_langinfo_l(CODESET, o));
	freelocale(o);

	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(setlocale(LC_ALL, all) != NULL, "setlocale(LC_ALL, \"%s\") did not accept the LC_ALL string", all);
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after restoring from the LC_ALL string the code set is \"%s\"", nl_langinfo(CODESET));
	pc_pass();
}
