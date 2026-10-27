/*
 * A returned string is not overwritten or invalidated by later calls of the family or by uselocale().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.006
 */
#include "gt.h"

int main(void)
{
	static char a[] = "pc alpha message";
	static char b[] = "pc beta message";
	static char c1[] = "pc gamma one";
	static char c2[] = "pc gamma many";
	char *ra, *rb, *rc;
	locale_t lo, old;

	pc_begin("XSH.gettext.006");
	lo = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (lo == (locale_t)0)
		lo = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (lo == (locale_t)0)
		pc_unresolved("newlocale for the POSIX locale failed");
	pc_test();
	ra = gettext(a);
	rb = dgettext("pc-dom", b);
	rc = ngettext(c1, c2, 5UL);
	(void)gettext(b);
	(void)dcgettext("pc-dom", c1, LC_TIME);
	(void)dngettext("pc-dom", c1, c2, 1UL);
	old = uselocale(lo);
	(void)gettext(a);
	if (old != (locale_t)0)
		(void)uselocale(old);
	pc_check(strcmp(ra, "pc alpha message") == 0, "first result changed to \"%s\"", ra);
	pc_check(strcmp(rb, "pc beta message") == 0, "second result changed to \"%s\"", rb);
	pc_check(strcmp(rc, "pc gamma many") == 0, "third result changed to \"%s\"", rc);
	freelocale(lo);
	pc_pass();
}
