/*
 * strcoll_l() uses its object and strcoll() the thread's locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.006
 */
#include "coll.h"

static void quick(const char *w)
{
	pc_check(coll_cmp("B", "a") < 0, "%s: \"B\" did not collate before \"a\"", w);
	pc_check(coll_cmp("Z", "a") < 0, "%s: \"Z\" did not collate before \"a\"", w);
	pc_check(coll_cmp("a", "B") > 0, "%s: \"a\" did not collate after \"B\"", w);
	pc_check(coll_cmp("a", "b") < 0, "%s: \"a\" did not collate before \"b\"", w);
}

int main(void)
{
	locale_t obj;

	pc_begin("XSH.strcoll.006");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	coll_obj = obj;
	quick("strcoll_l with the global locale changed");
	if (uselocale(obj) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	coll_obj = (locale_t)0;
	quick("strcoll with the POSIX object installed");
	(void)uselocale(LC_GLOBAL_LOCALE);
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	pc_pass();
}
