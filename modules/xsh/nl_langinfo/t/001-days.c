/*
 * Day names in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.001
 */
#include "lc.h"

int main(void)
{
	int i;

	pc_begin("XSH.nl_langinfo.001");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 7; i++) {
		pc_check(strcmp(nl_langinfo(lc_day[i]), lc_dayname[i]) == 0, "DAY_%d is \"%s\", expected \"%s\"", i + 1, nl_langinfo(lc_day[i]), lc_dayname[i]);
		pc_check(strcmp(nl_langinfo(lc_abday[i]), lc_abdayname[i]) == 0, "ABDAY_%d is \"%s\", expected \"%s\"", i + 1, nl_langinfo(lc_abday[i]), lc_abdayname[i]);
	}
	pc_pass();
}
