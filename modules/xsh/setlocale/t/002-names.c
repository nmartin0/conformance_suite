/*
 * "C" and "POSIX" both select the POSIX locale, for LC_ALL and each category.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.002
 */
#include "lc.h"

int main(void)
{
	static const char *const nm[2] = { "C", "POSIX" };
	int have, k, i, cat;
	char w[64];

	pc_begin("XSH.setlocale.002");
	lc_init();
	have = lc_find();
	pc_test();
	for (k = 0; k < 2; k++)
		for (i = 0; i < 7; i++) {
			cat = i < 6 ? lc_cat[i] : LC_ALL;
			if (have && setlocale(cat, lc_loc) == NULL)
				pc_unresolved("could not select the second locale for category %d", cat);
			snprintf(w, sizeof w, "setlocale(%d, \"%s\")", cat, nm[k]);
			pc_check(setlocale(cat, nm[k]) != NULL, "%s returned a null pointer", w);
			lc_check_cat((locale_t)0, cat, w);
			if (cat == LC_ALL)
				lc_check_items((locale_t)0, w);
			pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
		}
	pc_pass();
}
