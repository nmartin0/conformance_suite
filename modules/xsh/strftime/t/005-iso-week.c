/*
 * %G, %g and %V: the ISO 8601 week-based year.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.005
 * MUTANT    strftime.G_calendar
 * MUTANT    strftime.V_as_W
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

/* strftime() of fmt for tm gives exactly want, and returns its length. */
static void expect(const char *fmt, const struct tm *tm, const char *want)
{
	char buf[512];
	size_t n;

	memset(buf, 0x7f, sizeof buf);
	n = strftime(buf, sizeof buf, fmt, tm);
	pc_check(n < sizeof buf && n == strlen(want) && memcmp(buf, want, n + 1) == 0,
	    "strftime(\"%s\") for %d-%02d-%02d %02d:%02d:%02d (wday %d, yday %d) gave \"%.*s\" (return %zu), expected \"%s\"",
	    fmt, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
	    tm->tm_wday, tm->tm_yday, (int)(n < sizeof buf ? n : 0), buf, n, want);
}

/* Weeks in the ISO year y: 53 when 1 January is a Thursday, or a Wednesday in a leap year. */
static int iso_weeks(long long y)
{
	int jan1 = (int)(((days_from_civil(y, 1, 1) % 7) + 11) % 7);

	return jan1 == 4 || (jan1 == 3 && leap(y)) ? 53 : 52;
}

int main(void)
{
	char want[32];
	struct tm tm;
	int y, m, d, w, gy, isoday;

	pc_begin("XSH.strftime.005");
	pc_test();
	tm = mk(1999, 1, 2, 12, 0, 0);
	expect("%G %g %V", &tm, "1998 98 53");
	tm = mk(1997, 12, 30, 12, 0, 0);
	expect("%G %g %V", &tm, "1998 98 01");
	for (y = 1990; y <= 2030; y++)
		for (m = 1; m <= 12; m++)
			for (d = 1; d <= mdays(y, m - 1); d++) {
				tm = mk(y, m, d, 12, 0, 0);
				isoday = tm.tm_wday == 0 ? 7 : tm.tm_wday;
				w = (tm.tm_yday + 1 - isoday + 10) / 7;
				gy = y;
				if (w < 1) {
					gy = y - 1;
					w = iso_weeks(gy);
				} else if (w > iso_weeks(y)) {
					gy = y + 1;
					w = 1;
				}
				snprintf(want, sizeof want, "%d %02d %02d", gy, gy % 100, w);
				expect("%G %g %V", &tm, want);
			}
	pc_pass();
}
