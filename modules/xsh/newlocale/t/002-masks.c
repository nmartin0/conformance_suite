/*
 * Each category mask, and combinations, are accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.002
 */
#include "lc.h"
#include <ctype.h>

/* A handle is usable: the character class functions accept it and give the answers of the POSIX locale. */
static void usable(locale_t obj, const char *what)
{
	pc_check(toupper_l('a', obj) == 'A' && isdigit_l('7', obj) != 0 && isdigit_l('a', obj) == 0,
	    "the handle from newlocale(%s) does not behave as the POSIX locale", what);
}

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
		usable(obj, "one mask");
		freelocale(obj);
	}
	m = 0;
	for (i = 0; i < 6; i++)
		m |= lc_mask[i];
	obj = newlocale(m, "C", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale() with the six masks ORed failed, errno %s", pc_errname(errno));
	usable(obj, "the six masks");
	freelocale(obj);
	obj = newlocale(LC_CTYPE_MASK | LC_TIME_MASK, "C", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale(LC_CTYPE_MASK | LC_TIME_MASK, ...) failed, errno %s", pc_errname(errno));
	usable(obj, "two masks");
	freelocale(obj);
	pc_pass();
}
