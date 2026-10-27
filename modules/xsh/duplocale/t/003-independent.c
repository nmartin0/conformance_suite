/*
 * Changing the original leaves the copy alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.duplocale.003
 */
#include "lc.h"

int main(void)
{
	locale_t o, d, o2;

	pc_begin("XSH.duplocale.003");
	lc_init();
	lc_need();
	o = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (o == (locale_t)0)
		pc_unresolved("newlocale() could not make an object from the second locale");
	pc_test();
	d = duplocale(o);
	pc_check(d != (locale_t)0, "duplocale() failed, errno %s", pc_errname(errno));
	o2 = newlocale(LC_CTYPE_MASK, "C", o);
	if (o2 == (locale_t)0)
		pc_unresolved("newlocale() could not modify the original");
	pc_check(strcmp(nl_langinfo_l(CODESET, o2), lc_c0) == 0, "the modified object's code set is \"%s\", expected the POSIX one", nl_langinfo_l(CODESET, o2));
	pc_check(strcmp(nl_langinfo_l(CODESET, d), "UTF-8") == 0, "the copy's code set changed to \"%s\" when the original was modified", nl_langinfo_l(CODESET, d));
	freelocale(o2);
	freelocale(d);
	pc_pass();
}
