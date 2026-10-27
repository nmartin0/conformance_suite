/*
 * No function of the family changes errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.005
 */
#include "gt.h"

#define CHK(call, name) do { \
		errno = EDOM; \
		(void)(call); \
		pc_check(errno == EDOM, name " changed errno to %s", pc_errname(errno)); \
	} while (0)

int main(void)
{
	static char one[] = "pc one thing";
	static char many[] = "pc many things";
	locale_t lo;

	pc_begin("XSH.gettext.005");
	lo = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	if (lo == (locale_t)0)
		lo = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (lo == (locale_t)0)
		pc_unresolved("newlocale for the POSIX locale failed");
	pc_test();
	CHK(gettext(one), "gettext");
	CHK(dgettext("pc-dom", one), "dgettext");
	CHK(dcgettext("pc-dom", one, LC_MESSAGES), "dcgettext");
	CHK(ngettext(one, many, 1UL), "ngettext");
	CHK(ngettext(one, many, 4UL), "ngettext");
	CHK(dngettext("pc-dom", one, many, 2UL), "dngettext");
	CHK(dcngettext("pc-dom", one, many, 1UL, LC_MESSAGES), "dcngettext");
	CHK(gettext_l(one, lo), "gettext_l");
	CHK(dgettext_l("pc-dom", one, lo), "dgettext_l");
	CHK(dcgettext_l("pc-dom", one, LC_MESSAGES, lo), "dcgettext_l");
	CHK(ngettext_l(one, many, 3UL, lo), "ngettext_l");
	CHK(dngettext_l("pc-dom", one, many, 1UL, lo), "dngettext_l");
	CHK(dcngettext_l("pc-dom", one, many, 5UL, LC_MESSAGES, lo), "dcngettext_l");
	freelocale(lo);
	pc_pass();
}
