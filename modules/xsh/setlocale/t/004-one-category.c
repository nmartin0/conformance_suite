/*
 * Setting one category leaves the other five alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.004
 */
#include "lc.h"

int main(void)
{
	char before[6][512], after[512], qq[512];
	const char *r;
	int c, d;

	pc_begin("XSH.setlocale.004");
	lc_init();
	lc_need();
	pc_test();
	for (c = 0; c < 6; c++) {
		if (setlocale(LC_ALL, "C") == NULL)
			pc_unresolved("setlocale(LC_ALL, \"C\") failed");
		for (d = 0; d < 6; d++)
			lc_q(lc_cat[d], before[d], sizeof before[d]);
		r = setlocale(lc_cat[c], lc_loc);
		if (r == NULL)
			pc_unresolved("could not select the second locale for category %d", lc_cat[c]);
		lc_cp(qq, sizeof qq, r);
		pc_check(setlocale(lc_cat[c], qq) != NULL, "setlocale(%d, <the string the setting call returned>) failed", lc_cat[c]);
		for (d = 0; d < 6; d++) {
			if (d == c)
				continue;
			lc_q(lc_cat[d], after, sizeof after);
			pc_check(strcmp(before[d], after) == 0, "setting category %d changed the name of category %d from \"%s\" to \"%s\"", lc_cat[c], lc_cat[d], before[d], after);
			lc_check_cat((locale_t)0, lc_cat[d], "an untouched category");
		}
		if (lc_cat[c] == LC_CTYPE)
			pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after setting LC_CTYPE the code set is \"%s\"", nl_langinfo(CODESET));
	}
	pc_pass();
}
