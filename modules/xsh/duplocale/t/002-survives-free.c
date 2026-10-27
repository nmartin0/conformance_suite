/*
 * The copy outlives the original.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.duplocale.002
 */
#include "lc.h"

int main(void)
{
	locale_t o, d;
	int have;

	pc_begin("XSH.duplocale.002");
	lc_init();
	have = lc_find();
	o = have ? newlocale(LC_CTYPE_MASK, lc_loc, (locale_t)0) : newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (o == (locale_t)0)
		pc_unresolved("newlocale() could not make the object");
	pc_test();
	d = duplocale(o);
	pc_check(d != (locale_t)0, "duplocale() failed, errno %s", pc_errname(errno));
	freelocale(o);
	if (have) {
		pc_check(strcmp(nl_langinfo_l(CODESET, d), "UTF-8") == 0, "after freeing the original the copy's code set is \"%s\"", nl_langinfo_l(CODESET, d));
		lc_check_cat(d, LC_TIME, "the copy");
		lc_check_cat(d, LC_NUMERIC, "the copy");
	} else {
		lc_check_items(d, "the copy");
	}
	freelocale(d);
	pc_pass();
}
