/*
 * A name that no locale has: null return, nothing changed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.006
 */
#include "lc.h"

static void same(const char *all, char names[6][512], int have, const char *w)
{
	char b[512];
	int i;

	lc_q(LC_ALL, b, sizeof b);
	pc_check(strcmp(all, b) == 0, "%s: the name of LC_ALL changed from \"%s\" to \"%s\"", w, all, b);
	for (i = 0; i < 6; i++) {
		lc_q(lc_cat[i], b, sizeof b);
		pc_check(strcmp(names[i], b) == 0, "%s: the name of category %d changed from \"%s\" to \"%s\"", w, lc_cat[i], names[i], b);
	}
	if (have)
		pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "%s: the code set became \"%s\"", w, nl_langinfo(CODESET));
	else
		lc_check_cat((locale_t)0, LC_ALL, w);
}

int main(void)
{
	char all[512], names[6][512];
	int have, i;

	pc_begin("XSH.setlocale.006");
	lc_init();
	have = lc_find();
	lc_nosuch();
	if (setlocale(LC_ALL, "C") == NULL || (have && setlocale(LC_CTYPE, lc_loc) == NULL))
		pc_unresolved("could not set up the starting locale");
	lc_q(LC_ALL, all, sizeof all);
	for (i = 0; i < 6; i++)
		lc_q(lc_cat[i], names[i], sizeof names[i]);
	pc_test();
	pc_check(setlocale(LC_ALL, LC_NOSUCH) == NULL, "setlocale(LC_ALL, <missing name>) did not return a null pointer");
	same(all, names, have, "after a failed LC_ALL");
	for (i = 0; i < 6; i++) {
		pc_check(setlocale(lc_cat[i], LC_NOSUCH) == NULL, "setlocale(%d, <missing name>) did not return a null pointer", lc_cat[i]);
		same(all, names, have, "after a failed category");
	}
	pc_pass();
}
