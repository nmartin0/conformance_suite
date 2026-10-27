/*
 * duplocale() makes a distinct copy with the same data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.duplocale.001
 */
#include "lc.h"

int main(void)
{
	locale_t o, d;
	int have;

	pc_begin("XSH.duplocale.001");
	lc_init();
	have = lc_find();
	o = have ? newlocale(LC_CTYPE_MASK, lc_loc, (locale_t)0) : newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (o == (locale_t)0)
		pc_unresolved("newlocale() could not make the object");
	pc_test();
	d = duplocale(o);
	pc_check(d != (locale_t)0, "duplocale() failed, errno %s", pc_errname(errno));
	pc_check(d != o, "duplocale() returned the handle of the original");
	if (have) {
		pc_check(strcmp(nl_langinfo_l(CODESET, d), "UTF-8") == 0, "the copy's code set is \"%s\", the original's is \"UTF-8\"", nl_langinfo_l(CODESET, d));
		lc_check_cat(d, LC_TIME, "the copy");
		lc_check_cat(d, LC_NUMERIC, "the copy");
		lc_check_cat(d, LC_MESSAGES, "the copy");
		lc_check_cat(d, LC_COLLATE, "the copy");
	} else {
		lc_check_items(d, "the copy");
	}
	freelocale(d);
	freelocale(o);
	pc_pass();
}
