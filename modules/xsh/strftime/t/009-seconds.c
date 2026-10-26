/*
 * %s gives the seconds since the Epoch as mktime() computes them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.009
 * MUTANT    strftime.s_utc
 * MUTANT    strftime.s_isdst_ignored
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

static void seconds(const char *tz, int isdst, long long want)
{
	struct tm tm = mk(2021, 7, 15, 12, 0, 0);	/* zeroed, then filled member by member */
	char w[32];

	if (setenv("TZ", tz, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	tm.tm_isdst = isdst;
	snprintf(w, sizeof w, "%lld", want);
	expect("%s", &tm, w);
}

int main(void)
{
	long long noon = secs(2021, 7, 15, 12, 0, 0);

	pc_begin("XSH.strftime.009");
	pc_test();
	seconds("PCT3", 0, noon + 10800);
	seconds("PCS5PCD,M3.2.0,M11.1.0", 0, noon + 18000);
	seconds("PCS5PCD,M3.2.0,M11.1.0", 1, noon + 14400);
	pc_pass();
}
