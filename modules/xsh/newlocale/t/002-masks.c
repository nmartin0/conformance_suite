/*
 * Each category mask, and combinations, are accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.002
 */
#include "lc.h"

int main(void)
{
	locale_t obj;
	int m, i;

	pc_begin("XSH.newlocale.002");
	lc_init();
	pc_test();
	for (i = 0; i < 6; i++) {
		obj = newlocale(lc_mask[i], "C", (locale_t)0);
		pc_check(obj != (locale_t)0, "newlocale(mask of category %d, \"C\", 0) failed, errno %s", lc_cat[i], pc_errname(errno));
		freelocale(obj);
	}
	m = 0;
	for (i = 0; i < 6; i++)
		m |= lc_mask[i];
	obj = newlocale(m, "C", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale() with the six masks ORed failed, errno %s", pc_errname(errno));
	freelocale(obj);
	obj = newlocale(LC_CTYPE_MASK | LC_TIME_MASK, "C", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale(LC_CTYPE_MASK | LC_TIME_MASK, ...) failed, errno %s", pc_errname(errno));
	freelocale(obj);
	pc_pass();
}
