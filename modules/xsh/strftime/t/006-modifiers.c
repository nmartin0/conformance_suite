/*
 * E and O modifiers in the POSIX locale act as the plain specifiers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.006
 * MUTANT    strftime.E_empty
 * MUTANT    strftime.O_space
 * MUTANT    strftime.OB_abbrev
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Days from 1970-01-01 to y-m-d in the proleptic Gregorian calendar (m 1..12). */
static long long days_from_civil(long long y, int m, int d)
{
	long long era, yoe, doy, doe;

	y -= m <= 2;
	era = (y >= 0 ? y : y - 399) / 400;
	yoe = y - era * 400;
	doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
	doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
	return era * 146097 + doe - 719468;
}

static int leap(long long y)
{
	return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

static int mdays(long long y, int mon0)
{
	static const int n[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	return n[mon0] + (mon0 == 1 && leap(y));
}

/* Seconds of y-m-d h:mi:s counted as if UTC (m 1..12; any year). */
static long long secs(long long y, int m, int d, int h, int mi, int s)
{
	return days_from_civil(y, m, d) * 86400 + h * 3600LL + mi * 60LL + s;
}

/* A broken-down time for y-m-d h:mi:s (m 1..12), with tm_wday and tm_yday. */
static struct tm mk(long long y, int m, int d, int h, int mi, int s)
{
	struct tm tm;
	long long days = days_from_civil(y, m, d);

	memset(&tm, 0, sizeof tm);
	tm.tm_year = (int)(y - 1900);
	tm.tm_mon = m - 1;
	tm.tm_mday = d;
	tm.tm_hour = h;
	tm.tm_min = mi;
	tm.tm_sec = s;
	tm.tm_yday = (int)(days - days_from_civil(y, 1, 1));
	tm.tm_wday = (int)(((days % 7) + 11) % 7);
	tm.tm_isdst = 0;
	return tm;
}

int main(void)
{
	static const char *const pairs[][2] = {
		{ "%Ec", "%c" }, { "%EC", "%C" }, { "%Ex", "%x" }, { "%EX", "%X" }, { "%Ey", "%y" },
		{ "%EY", "%Y" }, { "%Ob", "%b" }, { "%OB", "%B" }, { "%Od", "%d" }, { "%Oe", "%e" },
		{ "%OH", "%H" }, { "%OI", "%I" }, { "%Om", "%m" }, { "%OM", "%M" }, { "%OS", "%S" },
		{ "%Ou", "%u" }, { "%OU", "%U" }, { "%OV", "%V" }, { "%Ow", "%w" }, { "%OW", "%W" },
		{ "%Oy", "%y" },
	};
	struct tm dates[3];
	char a[256], b[256];
	size_t i, j, na, nb;

	pc_begin("XSH.strftime.006");
	dates[0] = mk(2021, 1, 2, 3, 4, 5);
	dates[1] = mk(2021, 12, 25, 15, 30, 0);
	dates[2] = mk(1999, 1, 2, 23, 59, 59);
	pc_test();
	for (j = 0; j < 3; j++)
		for (i = 0; i < sizeof pairs / sizeof pairs[0]; i++) {
			memset(a, 1, sizeof a);
			memset(b, 2, sizeof b);
			na = strftime(a, sizeof a, pairs[i][0], &dates[j]);
			nb = strftime(b, sizeof b, pairs[i][1], &dates[j]);
			pc_check(nb > 0, "strftime(\"%s\") gave nothing", pairs[i][1]);
			pc_check(na == nb && memcmp(a, b, nb + 1) == 0,
			    "strftime(\"%s\") gave \"%.*s\" (return %zu), \"%s\" gave \"%s\" (return %zu)",
			    pairs[i][0], (int)(na < sizeof a ? na : 0), a, na, pairs[i][1], b, nb);
		}
	pc_pass();
}
