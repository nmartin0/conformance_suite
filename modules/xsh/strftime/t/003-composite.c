/*
 * The composite conversions, %n, %t, %% and ordinary characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.003
 * MUTANT    strftime.c_order
 * MUTANT    strftime.D_dmy
 * MUTANT    strftime.r_24
 * MUTANT    strftime.x_long
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

int main(void)
{
	struct tm tm;

	pc_begin("XSH.strftime.003");
	pc_test();
	tm = mk(2021, 1, 2, 3, 4, 5);	/* a Saturday */
	expect("%c", &tm, "Sat Jan  2 03:04:05 2021");
	expect("%D", &tm, "01/02/21");
	expect("%F", &tm, "2021-01-02");
	expect("%r", &tm, "03:04:05 AM");
	expect("%R", &tm, "03:04");
	expect("%T", &tm, "03:04:05");
	expect("%x", &tm, "01/02/21");
	expect("%X", &tm, "03:04:05");
	tm = mk(2021, 12, 25, 15, 30, 0);
	expect("%c", &tm, "Sat Dec 25 15:30:00 2021");
	expect("%r", &tm, "03:30:00 PM");
	expect("%D", &tm, "12/25/21");
	expect("%x", &tm, "12/25/21");
	expect("%n", &tm, "\n");
	expect("%t", &tm, "\t");
	expect("%%", &tm, "%");
	expect("a%%b c[%R]", &tm, "a%b c[15:30]");
	expect("", &tm, "");
	pc_pass();
}
