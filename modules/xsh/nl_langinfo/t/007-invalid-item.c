/*
 * An invalid item gives an empty string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.007
 */
#include "items.h"

static int good;

static void try(locale_t obj, const char *w)
{
	static const long cand[2] = { -1, INT_MAX };
	const char *p;
	size_t i;
	int k, known;

	for (k = 0; k < 2; k++) {
		known = 0;
		for (i = 0; i < IT_N; i++)
			if ((long)it_all[i] == cand[k])
				known = 1;
		if (known)
			continue;
		p = nl_langinfo((nl_item)cand[k]);
		pc_check(p != NULL, "%s: nl_langinfo(%ld) returned a null pointer", w, cand[k]);
		if (p[0] == '\0')
			good++;
		p = nl_langinfo_l((nl_item)cand[k], obj);
		pc_check(p != NULL, "%s: nl_langinfo_l(%ld) returned a null pointer", w, cand[k]);
		if (p[0] == '\0')
			good++;
	}
}

int main(void)
{
	locale_t obj, objl = (locale_t)0;
	int have;

	pc_begin("XSH.nl_langinfo.007");
	lc_init();
	have = lc_find();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	if (have) {
		objl = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
		if (objl == (locale_t)0)
			pc_unresolved("newlocale() could not make a locale object for the second locale");
	}
	pc_test();
	try(obj, "POSIX locale");
	if (have) {
		if (setlocale(LC_ALL, lc_loc) == NULL)
			pc_unresolved("could not select the second locale");
		try(objl, "second locale");
		(void)setlocale(LC_ALL, "C");
	}
	if (good == 0)
		pc_unresolved("none of the unlisted item values gave an empty string; they may be extension items");
	pc_pass();
}
