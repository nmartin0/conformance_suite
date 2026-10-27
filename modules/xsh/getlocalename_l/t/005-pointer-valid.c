/*
 * A string obtained for an object keeps its content.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlocalename_l.005
 */
#include "lc.h"

int main(void)
{
	char c1[512], c2[512], ca[512];
	const char *p1, *p2, *pa;
	locale_t obj, other, copy;
	int have, i;

	pc_begin("XSH.getlocalename_l.005");
	lc_init();
	have = lc_find();
	obj = have ? newlocale(LC_CTYPE_MASK, lc_loc, (locale_t)0) : newlocale(LC_ALL_MASK, "C", (locale_t)0);
	other = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (obj == (locale_t)0 || other == (locale_t)0 || setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("could not set up the objects");
	pc_test();
	p1 = getlocalename_l(LC_CTYPE, obj);
	p2 = getlocalename_l(LC_TIME, obj);
	pa = getlocalename_l(LC_ALL, obj);
	pc_check(p1 != NULL && p2 != NULL && pa != NULL, "getlocalename_l() returned a null pointer");
	lc_cp(c1, sizeof c1, p1);
	lc_cp(c2, sizeof c2, p2);
	lc_cp(ca, sizeof ca, pa);
	for (i = 0; i < 6; i++) {
		(void)getlocalename_l(lc_cat[i], obj);
		(void)getlocalename_l(lc_cat[i], other);
		(void)getlocalename_l(lc_cat[i], LC_GLOBAL_LOCALE);
	}
	(void)getlocalename_l(LC_ALL, other);
	(void)getlocalename_l(LC_ALL, LC_GLOBAL_LOCALE);
	(void)setlocale(LC_ALL, "C");
	(void)setlocale(LC_CTYPE, NULL);
	(void)nl_langinfo(CODESET);
	copy = duplocale(obj);
	if (copy != (locale_t)0) {
		(void)getlocalename_l(LC_CTYPE, copy);
		(void)uselocale(copy);
		(void)getlocalename_l(LC_CTYPE, uselocale((locale_t)0));
		(void)uselocale(LC_GLOBAL_LOCALE);
		freelocale(copy);
	}
	pc_check(strcmp(p1, c1) == 0, "the LC_CTYPE name changed from \"%s\" to \"%s\" after other calls", c1, p1);
	pc_check(strcmp(p2, c2) == 0, "the LC_TIME name changed from \"%s\" to \"%s\" after other calls", c2, p2);
	pc_check(strcmp(pa, ca) == 0, "the LC_ALL string changed from \"%s\" to \"%s\" after other calls", ca, pa);
	freelocale(other);
	freelocale(obj);
	pc_pass();
}
