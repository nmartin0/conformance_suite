/*
 * A query with a null locale returns a string and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.003
 */
#include "lc.h"

int main(void)
{
	char a[512], b[512], all[512], cats[6][512];
	const char *q;
	int have, i, cat;

	pc_begin("XSH.setlocale.003");
	lc_init();
	have = lc_find();
	if (have && (setlocale(LC_CTYPE, lc_loc) == NULL || setlocale(LC_TIME, lc_loc) == NULL))
		pc_unresolved("could not select the second locale");
	pc_test();
	q = setlocale(LC_ALL, NULL);
	pc_check(q != NULL, "setlocale(LC_ALL, NULL) returned a null pointer");
	lc_cp(all, sizeof all, q);
	for (i = 0; i < 6; i++) {
		q = setlocale(lc_cat[i], NULL);
		pc_check(q != NULL, "setlocale(%d, NULL) returned a null pointer", lc_cat[i]);
		lc_cp(cats[i], sizeof cats[i], q);
	}
	for (i = 0; i < 7; i++) {
		cat = i < 6 ? lc_cat[i] : LC_ALL;
		lc_q(cat, a, sizeof a);
		lc_q(cat, b, sizeof b);
		pc_check(strcmp(a, b) == 0, "two queries of category %d gave \"%s\" and \"%s\"", cat, a, b);
		pc_check(strcmp(a, i < 6 ? cats[i] : all) == 0, "a later query of category %d gave \"%s\", an earlier one \"%s\"", cat, a, i < 6 ? cats[i] : all);
	}
	if (have)
		pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "queries changed the code set to \"%s\"", nl_langinfo(CODESET));
	else
		lc_check_cat((locale_t)0, LC_ALL, "after queries");
	pc_pass();
}
