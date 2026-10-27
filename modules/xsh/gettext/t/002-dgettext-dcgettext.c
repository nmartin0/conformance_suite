/*
 * In the POSIX locale dgettext() and dcgettext() return msgid whatever the domain and category.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.002
 */
#include "gt.h"

int main(void)
{
	static char m[] = "pc domain message";
	static const int cats[] = { LC_MESSAGES, LC_CTYPE, LC_TIME, LC_COLLATE, LC_MONETARY, LC_NUMERIC };
	static const char *const doms[] = { "pc-dom", "messages", "", NULL };
	size_t i, j;

	pc_begin("XSH.gettext.002");
	pc_test();
	for (i = 0; i < sizeof doms / sizeof doms[0]; i++) {
		pc_check(dgettext(doms[i], m) == m, "dgettext(\"%s\") did not return msgid", doms[i] ? doms[i] : "(null)");
		for (j = 0; j < sizeof cats / sizeof cats[0]; j++)
			pc_check(dcgettext(doms[i], m, cats[j]) == m, "dcgettext(\"%s\", category %d) did not return msgid",
			    doms[i] ? doms[i] : "(null)", cats[j]);
	}
	pc_pass();
}
