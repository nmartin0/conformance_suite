/*
 * Names taken from an object with LC_CTYPE from another locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlocalename_l.004
 */
#include "lc.h"

int main(void)
{
	char ct[512], tm[512], all[512];
	locale_t obj, o;
	const char *p;

	pc_begin("XSH.getlocalename_l.004");
	lc_init();
	lc_need();
	obj = newlocale(LC_CTYPE_MASK, lc_loc, (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make an object with LC_CTYPE from the second locale");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	p = getlocalename_l(LC_CTYPE, obj);
	pc_check(p != NULL, "getlocalename_l(LC_CTYPE, object) returned a null pointer");
	lc_cp(ct, sizeof ct, p);
	p = getlocalename_l(LC_TIME, obj);
	pc_check(p != NULL, "getlocalename_l(LC_TIME, object) returned a null pointer");
	lc_cp(tm, sizeof tm, p);
	p = getlocalename_l(LC_ALL, obj);
	pc_check(p != NULL, "getlocalename_l(LC_ALL, object) returned a null pointer");
	lc_cp(all, sizeof all, p);

	o = newlocale(LC_CTYPE_MASK, ct, (locale_t)0);
	pc_check(o != (locale_t)0, "newlocale() did not accept the LC_CTYPE name \"%s\", errno %s", ct, pc_errname(errno));
	pc_check(strcmp(nl_langinfo_l(CODESET, o), "UTF-8") == 0, "the object made from the LC_CTYPE name has code set \"%s\"", nl_langinfo_l(CODESET, o));
	freelocale(o);
	o = newlocale(LC_CTYPE_MASK, tm, (locale_t)0);
	pc_check(o != (locale_t)0, "newlocale() did not accept the LC_TIME name \"%s\", errno %s", tm, pc_errname(errno));
	pc_check(strcmp(nl_langinfo_l(CODESET, o), lc_c0) == 0, "the object made from the LC_TIME name has code set \"%s\", expected the POSIX one", nl_langinfo_l(CODESET, o));
	freelocale(o);

	pc_check(setlocale(LC_ALL, all) != NULL, "setlocale(LC_ALL, \"%s\") did not accept the LC_ALL string", all);
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after setlocale(LC_ALL, <string of the object>) the code set is \"%s\"", nl_langinfo(CODESET));
	lc_check_cat((locale_t)0, LC_TIME, "global locale set from the object's string");
	lc_check_cat((locale_t)0, LC_NUMERIC, "global locale set from the object's string");
	lc_check_cat((locale_t)0, LC_MESSAGES, "global locale set from the object's string");
	lc_check_cat((locale_t)0, LC_COLLATE, "global locale set from the object's string");
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	pc_pass();
}
