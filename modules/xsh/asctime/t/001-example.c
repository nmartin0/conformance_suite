/*
 * asctime() renders the broken-down time as a 26-byte string ending in a newline (the page's own example).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.asctime.001
 */
#include "pc.h"
#include <string.h>
#include <time.h>

int main(void)
{
	struct tm tm;
	char *p;

	pc_begin("XSH.asctime.001");
	pc_test();
	memset(&tm, 0, sizeof tm);
	tm.tm_sec = 52;
	tm.tm_min = 3;
	tm.tm_hour = 1;
	tm.tm_mday = 16;
	tm.tm_mon = 8;
	tm.tm_year = 73;
	tm.tm_wday = 0;
	tm.tm_yday = 258;
	p = asctime(&tm);
	pc_check(p != NULL, "asctime returned a null pointer");
	pc_check(strcmp(p, "Sun Sep 16 01:03:52 1973\n") == 0, "asctime produced \"%s\"", p);
	pc_check(strlen(p) == 25 && p[25] == '\0', "the string is not 25 characters and a null");
	pc_pass();
}
