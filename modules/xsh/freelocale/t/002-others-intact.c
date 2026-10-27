/*
 * Other objects survive freelocale().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freelocale.002
 */
#include "lc.h"

int main(void)
{
	locale_t a, b;
	int k;

	pc_begin("XSH.freelocale.002");
	lc_init();
	pc_test();
	for (k = 0; k < 2; k++) {
		a = newlocale(LC_ALL_MASK, "C", (locale_t)0);
		b = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
		if (a == (locale_t)0 || b == (locale_t)0)
			pc_unresolved("newlocale() could not make the objects");
		if (k == 0) {
			freelocale(a);
			lc_check_items(b, "the second object after freeing the first");
			freelocale(b);
		} else {
			freelocale(b);
			lc_check_items(a, "the first object after freeing the second");
			freelocale(a);
		}
	}
	pc_pass();
}
