/*
 * ctime() converts calendar times to the local date and time string, here with a zone of no offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctime.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	static const struct { long t; const char *want; } tab[] = {
		{ 86399L, "Thu Jan  1 23:59:59 1970\n" },
		{ 31536000L, "Fri Jan  1 00:00:00 1971\n" },
		{ 951782400L, "Tue Feb 29 00:00:00 2000\n" },
		{ 1000000000L, "Sun Sep  9 01:46:40 2001\n" },
		{ 1234567890L, "Fri Feb 13 23:31:30 2009\n" },
		{ 2147483647L, "Tue Jan 19 03:14:07 2038\n" },
	};
	size_t i;

	pc_begin("XSH.ctime.002");
	pc_check(setenv("TZ", "UTC0", 1) == 0, "setenv failed");
	tzset();
	pc_test();
	for (i = 0; i < sizeof tab / sizeof tab[0]; i++) {
		time_t t = (time_t)tab[i].t;
		char *p = ctime(&t);

		pc_check(p != NULL, "ctime returned a null pointer for %ld", tab[i].t);
		pc_check(strcmp(p, tab[i].want) == 0, "ctime(%ld) produced \"%s\", expected \"%s\"",
		    tab[i].t, p, tab[i].want);
	}
	pc_pass();
}
