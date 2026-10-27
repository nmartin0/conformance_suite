/*
 * Month names in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.002
 */
#include "lc.h"

int main(void)
{
	int i;

	pc_begin("XSH.nl_langinfo.002");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 12; i++) {
		pc_check(strcmp(nl_langinfo(lc_mon[i]), lc_monname[i]) == 0, "MON_%d is \"%s\", expected \"%s\"", i + 1, nl_langinfo(lc_mon[i]), lc_monname[i]);
		pc_check(strcmp(nl_langinfo(lc_abmon[i]), lc_abmonname[i]) == 0, "ABMON_%d is \"%s\", expected \"%s\"", i + 1, nl_langinfo(lc_abmon[i]), lc_abmonname[i]);
	}
	pc_pass();
}
