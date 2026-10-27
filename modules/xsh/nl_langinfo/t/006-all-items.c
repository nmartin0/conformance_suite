/*
 * Every listed item gives a string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.006
 */
#include "items.h"

int main(void)
{
	locale_t obj;
	const char *p;
	size_t i;

	pc_begin("XSH.nl_langinfo.006");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < IT_N; i++) {
		p = nl_langinfo(it_all[i]);
		pc_check(p != NULL, "nl_langinfo(item %zu of the list) returned a null pointer", i);
		pc_check(strlen(p) < 4096, "nl_langinfo(item %zu) returned a string of absurd length", i);
		p = nl_langinfo_l(it_all[i], obj);
		pc_check(p != NULL, "nl_langinfo_l(item %zu of the list) returned a null pointer", i);
		pc_check(strlen(p) < 4096, "nl_langinfo_l(item %zu) returned a string of absurd length", i);
	}
	freelocale(obj);
	pc_pass();
}
