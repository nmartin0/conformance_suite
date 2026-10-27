/*
 * Categories not in the mask come from the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.003
 */
#include "lc.h"

int main(void)
{
	locale_t obj;
	int c, d;

	pc_begin("XSH.newlocale.003");
	lc_init();
	lc_need();
	pc_test();
	for (c = 0; c < 6; c++) {
		obj = newlocale(lc_mask[c], lc_loc, (locale_t)0);
		if (obj == (locale_t)0)
			pc_unresolved("newlocale() could not make an object for category %d from the second locale", lc_cat[c]);
		if (lc_cat[c] == LC_CTYPE)
			pc_check(strcmp(nl_langinfo_l(CODESET, obj), "UTF-8") == 0, "LC_CTYPE_MASK object: the code set is \"%s\"", nl_langinfo_l(CODESET, obj));
		for (d = 0; d < 6; d++)
			if (d != c)
				lc_check_cat(obj, lc_cat[d], "a category not in the mask");
		freelocale(obj);
	}
	pc_pass();
}
