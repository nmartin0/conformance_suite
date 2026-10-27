/*
 * Hours, minutes and seconds are printed as two digits with a leading zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.asctime.004
 */
#include "pc.h"
#include <string.h>
#include <time.h>

int main(void)
{
	static const int hv[] = { 0, 9, 10, 23 };
	static const int mv[] = { 0, 3, 10, 59 };
	static const int sv[] = { 0, 7, 10, 59 };
	struct tm tm;
	char want[40];
	char *p;
	int a, b, c;

	pc_begin("XSH.asctime.004");
	pc_test();
	memset(&tm, 0, sizeof tm);
	tm.tm_mday = 12;
	tm.tm_mon = 5;
	tm.tm_year = 99;
	tm.tm_wday = 6;
	for (a = 0; a < 4; a++)
		for (b = 0; b < 4; b++)
			for (c = 0; c < 4; c++) {
				tm.tm_hour = hv[a];
				tm.tm_min = mv[b];
				tm.tm_sec = sv[c];
				memcpy(want, "Sat Jun 12 ", 11);
				want[11] = (char)('0' + hv[a] / 10);
				want[12] = (char)('0' + hv[a] % 10);
				want[13] = ':';
				want[14] = (char)('0' + mv[b] / 10);
				want[15] = (char)('0' + mv[b] % 10);
				want[16] = ':';
				want[17] = (char)('0' + sv[c] / 10);
				want[18] = (char)('0' + sv[c] % 10);
				memcpy(want + 19, " 1999\n", 7);
				p = asctime(&tm);
				pc_check(p != NULL, "asctime returned a null pointer");
				pc_check(strcmp(p, want) == 0, "got \"%s\", expected \"%s\"", p, want);
			}
	pc_pass();
}
