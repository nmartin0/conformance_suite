/*
 * A category that is not a category gives a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlocalename_l.002
 */
#include "lc.h"

int main(void)
{
	static const long cand[2] = { -1, INT_MAX };
	locale_t obj;
	int k, i, known, tried = 0;

	pc_begin("XSH.getlocalename_l.002");
	lc_init();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (k = 0; k < 2; k++) {
		known = cand[k] == LC_ALL;
		for (i = 0; i < 6; i++)
			if (cand[k] == lc_cat[i])
				known = 1;
		if (known)
			continue;
		tried++;
		pc_check(getlocalename_l((int)cand[k], obj) == NULL, "getlocalename_l(%ld, object) did not return a null pointer", cand[k]);
		pc_check(getlocalename_l((int)cand[k], LC_GLOBAL_LOCALE) == NULL, "getlocalename_l(%ld, LC_GLOBAL_LOCALE) did not return a null pointer", cand[k]);
	}
	if (tried == 0)
		pc_unresolved("both values tried are category values");
	freelocale(obj);
	pc_pass();
}
