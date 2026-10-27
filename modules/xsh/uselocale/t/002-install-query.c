/*
 * Install, query and uninstall return the expected handles.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uselocale.002
 */
#include "lc.h"

int main(void)
{
	locale_t o1, o2, p;

	pc_begin("XSH.uselocale.002");
	lc_init();
	o1 = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	o2 = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (o1 == (locale_t)0 || o2 == (locale_t)0)
		pc_unresolved("newlocale() could not make the locale objects");
	pc_test();
	p = uselocale(o1);
	pc_check(p == LC_GLOBAL_LOCALE, "the first install did not return LC_GLOBAL_LOCALE");
	pc_check(uselocale((locale_t)0) == o1, "uselocale(0) did not return the installed object");
	pc_check(uselocale((locale_t)0) == o1, "a second uselocale(0) did not return the installed object");
	p = uselocale(o2);
	pc_check(p == o1, "installing a second object did not return the first");
	pc_check(uselocale((locale_t)0) == o2, "uselocale(0) did not return the second object");
	p = uselocale(o2);
	pc_check(p == o2, "installing the installed object did not return it");
	p = uselocale(LC_GLOBAL_LOCALE);
	pc_check(p == o2, "installing LC_GLOBAL_LOCALE did not return the installed object");
	pc_check(uselocale((locale_t)0) == LC_GLOBAL_LOCALE, "uselocale(0) after uninstalling did not return LC_GLOBAL_LOCALE");
	freelocale(o1);
	freelocale(o2);
	pc_pass();
}
