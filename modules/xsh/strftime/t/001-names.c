/*
 * Day, month and AM/PM names in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.001
 * MUTANT    strftime.abday_full
 * MUTANT    strftime.abmon_full
 * MUTANT    strftime.pm_noon
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
	static const char *const ab[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char *const day[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
		"Saturday" };
	static const char *const abm[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
		"Nov", "Dec" };
	static const char *const mon[] = { "January", "February", "March", "April", "May", "June", "July",
		"August", "September", "October", "November", "December" };
	static const int hours[] = { 0, 11, 12, 23 };
	struct tm tm;
	int i;

	pc_begin("XSH.strftime.001");
	pc_test();
	for (i = 0; i < 7; i++) {
		tm = mk(2021, 10, 24 + i, 12, 0, 0);	/* 24 October 2021 is a Sunday */
		pc_check(tm.tm_wday == i, "test date error");
		expect("%a", &tm, ab[i]);
		expect("%A", &tm, day[i]);
	}
	for (i = 0; i < 12; i++) {
		tm = mk(2021, i + 1, 15, 12, 0, 0);
		expect("%b", &tm, abm[i]);
		expect("%h", &tm, abm[i]);
		expect("%B", &tm, mon[i]);
	}
	for (i = 0; i < 4; i++) {
		tm = mk(2021, 10, 24, hours[i], 30, 0);
		expect("%p", &tm, hours[i] < 12 ? "AM" : "PM");
	}
	pc_pass();
}
