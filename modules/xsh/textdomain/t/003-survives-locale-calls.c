/*
 * The domain stays in effect across setlocale(), uselocale() and gettext functions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.003
 */
#include "td.h"

static void still(const char *what)
{
	const char *r = textdomain(NULL);

	pc_check(r != NULL && strcmp(r, "pc-dom-keep") == 0, "after %s the domain is \"%s\"", what, r ? r : "(null)");
}

int main(void)
{
	locale_t lo, old;

	pc_begin("XSH.textdomain.003");
	lo = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (lo == (locale_t)0)
		lo = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (lo == (locale_t)0)
		pc_unresolved("newlocale failed");
	pc_check(textdomain("pc-dom-keep") != NULL, "textdomain failed");
	pc_test();
	(void)setlocale(LC_ALL, "C");
	still("setlocale(LC_ALL, \"C\")");
	(void)setlocale(LC_ALL, "POSIX");
	still("setlocale(LC_ALL, \"POSIX\")");
	(void)setlocale(LC_ALL, "");
	still("setlocale(LC_ALL, \"\")");
	(void)setlocale(LC_ALL, "C");
	old = uselocale(lo);
	still("uselocale()");
	if (old != (locale_t)0)
		(void)uselocale(old);
	still("uselocale() back");
	(void)gettext("pc message");
	(void)dgettext("pc-other-dom", "pc message");
	(void)ngettext("pc message", "pc messages", 2UL);
	still("gettext functions");
	freelocale(lo);
	pc_pass();
}
