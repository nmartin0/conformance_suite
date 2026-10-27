/*
 * nl_langinfo_l() with an object for "C".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.008
 */
#include "lc.h"

int main(void)
{
	locale_t obj;

	pc_begin("XSH.nl_langinfo.008");
	lc_init();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	lc_check_items(obj, "object for C");
	freelocale(obj);
	pc_pass();
}
