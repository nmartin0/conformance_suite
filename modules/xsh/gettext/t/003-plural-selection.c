/*
 * In the POSIX locale the n functions return msgid for n == 1 and msgid_plural otherwise.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.003
 */
#include "gt.h"

int main(void)
{
	static char one[] = "pc one thing";
	static char many[] = "pc many things";
	static const unsigned long ns[] = { 0UL, 1UL, 2UL, 3UL, 100UL, ULONG_MAX };
	size_t i;
	char *exp;

	pc_begin("XSH.gettext.003");
	pc_test();
	for (i = 0; i < sizeof ns / sizeof ns[0]; i++) {
		exp = ns[i] == 1UL ? one : many;
		pc_check(ngettext(one, many, ns[i]) == exp, "ngettext with n=%lu returned the wrong string", ns[i]);
		pc_check(dngettext("pc-dom", one, many, ns[i]) == exp, "dngettext with n=%lu returned the wrong string", ns[i]);
		pc_check(dcngettext("pc-dom", one, many, ns[i], LC_MESSAGES) == exp,
		    "dcngettext with n=%lu returned the wrong string", ns[i]);
		pc_check(dcngettext(NULL, one, many, ns[i], LC_TIME) == exp,
		    "dcngettext(NULL domain) with n=%lu returned the wrong string", ns[i]);
	}
	pc_pass();
}
