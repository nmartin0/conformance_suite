/*
 * The day of the month is padded to two characters with a space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.asctime.002
 */
#include "pc.h"
#include <string.h>
#include <time.h>

int main(void)
{
	struct tm tm;
	char want[32];
	char *p;
	int d;

	pc_begin("XSH.asctime.002");
	pc_test();
	memset(&tm, 0, sizeof tm);
	tm.tm_mon = 0;
	tm.tm_year = 100;
	tm.tm_wday = 1;
	for (d = 1; d <= 31; d++) {
		tm.tm_mday = d;
		memcpy(want, "Mon Jan ", 8);
		want[8] = d < 10 ? ' ' : (char)('0' + d / 10);
		want[9] = (char)('0' + d % 10);
		memcpy(want + 10, " 00:00:00 2000\n", 16);
		p = asctime(&tm);
		pc_check(p != NULL, "asctime returned a null pointer for day %d", d);
		pc_check(strcmp(p, want) == 0, "day %d: got \"%s\", expected \"%s\"", d, p, want);
	}
	pc_pass();
}
