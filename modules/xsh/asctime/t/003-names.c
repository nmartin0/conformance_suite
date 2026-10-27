/*
 * Weekday and month names come from the tm_wday and tm_mon members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.asctime.003
 */
#include "pc.h"
#include <string.h>
#include <time.h>

int main(void)
{
	static const char *const wd[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char *const mo[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	struct tm tm;
	char want[40];
	char *p;
	int w, m;

	pc_begin("XSH.asctime.003");
	pc_test();
	memset(&tm, 0, sizeof tm);
	tm.tm_mday = 10;
	tm.tm_hour = 12;
	tm.tm_year = 100;
	for (w = 0; w < 7; w++) {
		for (m = 0; m < 12; m++) {
			tm.tm_wday = w;
			tm.tm_mon = m;
			memcpy(want, wd[w], 3);
			want[3] = ' ';
			memcpy(want + 4, mo[m], 3);
			memcpy(want + 7, " 10 12:00:00 2000\n", 19);
			p = asctime(&tm);
			pc_check(p != NULL, "asctime returned a null pointer");
			pc_check(strcmp(p, want) == 0, "wday %d mon %d: got \"%s\", expected \"%s\"",
			    w, m, p, want);
		}
	}
	pc_pass();
}
