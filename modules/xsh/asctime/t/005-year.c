/*
 * The year is 1900 plus tm_year in four decimal digits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.asctime.005
 */
#include "pc.h"
#include <string.h>
#include <time.h>

int main(void)
{
	static const struct { int y; const char *want; } tab[] = {
		{ -900, "Mon Jan  3 04:05:06 1000\n" },
		{ 70, "Mon Jan  3 04:05:06 1970\n" },
		{ 100, "Mon Jan  3 04:05:06 2000\n" },
		{ 1, "Mon Jan  3 04:05:06 1901\n" },
		{ 8099, "Mon Jan  3 04:05:06 9999\n" },
	};
	struct tm tm;
	char *p;
	size_t i;

	pc_begin("XSH.asctime.005");
	pc_test();
	for (i = 0; i < sizeof tab / sizeof tab[0]; i++) {
		memset(&tm, 0, sizeof tm);
		tm.tm_sec = 6;
		tm.tm_min = 5;
		tm.tm_hour = 4;
		tm.tm_mday = 3;
		tm.tm_mon = 0;
		tm.tm_year = tab[i].y;
		tm.tm_wday = 1;
		p = asctime(&tm);
		pc_check(p != NULL, "asctime returned a null pointer for tm_year %d", tab[i].y);
		pc_check(strcmp(p, tab[i].want) == 0, "tm_year %d: got \"%s\", expected \"%s\"",
		    tab[i].y, p, tab[i].want);
	}
	pc_pass();
}
