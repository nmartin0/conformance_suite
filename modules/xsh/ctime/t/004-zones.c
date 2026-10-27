/*
 * ctime() gives local time: zones with an offset from UTC shift the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctime.004
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	static const struct { const char *tz; const char *want; } tab[] = {
		{ "EST5", "Wed Dec 31 19:00:00 1969\n" },
		{ "ABC-3", "Thu Jan  1 03:00:00 1970\n" },
		{ "NST3:30", "Wed Dec 31 20:30:00 1969\n" },
		{ "UTC0", "Thu Jan  1 00:00:00 1970\n" },
	};
	size_t i;

	pc_begin("XSH.ctime.004");
	pc_test();
	for (i = 0; i < sizeof tab / sizeof tab[0]; i++) {
		time_t t = 0;
		char *p;

		pc_check(setenv("TZ", tab[i].tz, 1) == 0, "setenv failed");
		tzset();
		p = ctime(&t);
		pc_check(p != NULL, "ctime returned a null pointer");
		pc_check(strcmp(p, tab[i].want) == 0, "TZ=%s: ctime(0) produced \"%s\", expected \"%s\"",
		    tab[i].tz, p, tab[i].want);
	}
	pc_pass();
}
