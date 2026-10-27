/*
 * strcoll_l() with an object for "C".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.004
 */
#include "coll.h"

int main(void)
{
	char a1[] = "abc", a2[] = "abc", c1[] = "", c2[] = "";
	locale_t obj;
	int r;

	pc_begin("XSH.strcoll.004");
	lc_init();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	r = strcoll_l(a1, a2, obj);
	pc_check(r == 0, "strcoll_l(\"abc\", \"abc\") returned %d", r);
	r = strcoll_l(c1, c2, obj);
	pc_check(r == 0, "strcoll_l(\"\", \"\") returned %d", r);
	coll_obj = obj;
	coll_check_singles("strcoll_l");
	coll_check_pairs("strcoll_l");
	freelocale(obj);
	pc_pass();
}
