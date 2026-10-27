/*
 * The _l functions with a POSIX locale object return msgid, or the right plural form.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.004
 */
#include "gt.h"

int main(void)
{
	static char one[] = "pc one thing";
	static char many[] = "pc many things";
	locale_t lo;

	pc_begin("XSH.gettext.004");
	lo = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (lo == (locale_t)0)
		lo = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (lo == (locale_t)0)
		pc_unresolved("newlocale for the POSIX locale failed");
	pc_test();
	pc_check(gettext_l(one, lo) == one, "gettext_l did not return msgid");
	pc_check(dgettext_l("pc-dom", one, lo) == one, "dgettext_l did not return msgid");
	pc_check(dcgettext_l("pc-dom", one, LC_MESSAGES, lo) == one, "dcgettext_l did not return msgid");
	pc_check(ngettext_l(one, many, 1UL, lo) == one, "ngettext_l n=1 did not return msgid");
	pc_check(ngettext_l(one, many, 2UL, lo) == many, "ngettext_l n=2 did not return msgid_plural");
	pc_check(dngettext_l("pc-dom", one, many, 1UL, lo) == one, "dngettext_l n=1 did not return msgid");
	pc_check(dngettext_l("pc-dom", one, many, 0UL, lo) == many, "dngettext_l n=0 did not return msgid_plural");
	pc_check(dcngettext_l("pc-dom", one, many, 1UL, LC_MESSAGES, lo) == one, "dcngettext_l n=1 did not return msgid");
	pc_check(dcngettext_l("pc-dom", one, many, 7UL, LC_MESSAGES, lo) == many,
	    "dcngettext_l n=7 did not return msgid_plural");
	freelocale(lo);
	pc_pass();
}
