/*
 * Names of the POSIX locale's categories, from an object and from the global locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlocalename_l.001
 */
#include "lc.h"

static void names(locale_t src, int have, const char *w)
{
	char n[512];
	const char *p;
	locale_t o;
	int i;

	for (i = 0; i < 6; i++) {
		p = getlocalename_l(lc_cat[i], src);
		pc_check(p != NULL, "%s: getlocalename_l(%d, ...) returned a null pointer", w, lc_cat[i]);
		lc_cp(n, sizeof n, p);
		o = newlocale(lc_mask[i], n, (locale_t)0);
		pc_check(o != (locale_t)0, "%s: newlocale() did not accept the name \"%s\" of category %d, errno %s", w, n, lc_cat[i], pc_errname(errno));
		lc_check_cat(o, lc_cat[i], w);
		freelocale(o);
	}
	p = getlocalename_l(LC_ALL, src);
	pc_check(p != NULL, "%s: getlocalename_l(LC_ALL, ...) returned a null pointer", w);
	lc_cp(n, sizeof n, p);
	if (setlocale(LC_ALL, have ? lc_loc : "C") == NULL)
		pc_unresolved("could not set the global locale");
	pc_check(setlocale(LC_ALL, n) != NULL, "%s: setlocale(LC_ALL, \"%s\") did not accept the LC_ALL string", w, n);
	lc_check_cat((locale_t)0, LC_ALL, w);
	lc_check_items((locale_t)0, w);
}

int main(void)
{
	locale_t obj;
	int have;

	pc_begin("XSH.getlocalename_l.001");
	lc_init();
	have = lc_find();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0 || setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("could not set up the POSIX locale and object");
	pc_test();
	names(obj, have, "object for C");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	names(LC_GLOBAL_LOCALE, have, "global locale");
	freelocale(obj);
	pc_pass();
}
