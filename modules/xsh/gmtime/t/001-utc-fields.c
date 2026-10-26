/*
 * gmtime() gives the UTC broken-down time of the XBD 4.19 expression.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gmtime.001
 * MUTANT    gmtime.local
 * MUTANT    gmtime.leap_2100
 * MUTANT    gmtime.wday
 * MUTANT    gmtime.isdst
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

/* The expression of XBD 4.19 (all integer divisions truncate). */
static long long expr419(const struct tm *tm)
{
	long long y = tm->tm_year;

	return tm->tm_sec + tm->tm_min * 60LL + tm->tm_hour * 3600LL + tm->tm_yday * 86400LL +
	    (y - 70) * 31536000LL + ((y - 69) / 4) * 86400LL - ((y - 1) / 100) * 86400LL +
	    ((y + 299) / 400) * 86400LL;
}

/*
 * Check that tm is the broken-down time whose XBD 4.19 value is v (v >= 0,
 * year >= 1970): members in range, the expression, the calendar date
 * matching tm_yday, and tm_wday counted from Thursday 1970-01-01.
 */
static void check_bd(const struct tm *tm, long long v, const char *what)
{
	long long y = 1900LL + tm->tm_year, days;

	pc_check(tm->tm_sec >= 0 && tm->tm_sec <= 60 && tm->tm_min >= 0 && tm->tm_min <= 59 &&
	    tm->tm_hour >= 0 && tm->tm_hour <= 23 && tm->tm_mon >= 0 && tm->tm_mon <= 11 &&
	    tm->tm_mday >= 1 && tm->tm_yday >= 0 && tm->tm_yday <= 365 && tm->tm_wday >= 0 &&
	    tm->tm_wday <= 6, "%s: a member is out of range (year %d mon %d mday %d %d:%d:%d wday %d yday %d)",
	    what, tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
	    tm->tm_wday, tm->tm_yday);
	pc_check(expr419(tm) == v, "%s: gave %lld-%02d-%02d %02d:%02d:%02d (yday %d), whose XBD 4.19 value is %lld, expected %lld",
	    what, y, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_yday,
	    expr419(tm), v);
	pc_check(tm->tm_mday <= mdays(y, tm->tm_mon), "%s: day %d of month %d does not exist",
	    what, tm->tm_mday, tm->tm_mon);
	days = days_from_civil(y, tm->tm_mon + 1, tm->tm_mday);
	pc_check(days - days_from_civil(y, 1, 1) == tm->tm_yday,
	    "%s: %lld-%02d-%02d is not day %d of its year", what, y, tm->tm_mon + 1, tm->tm_mday,
	    tm->tm_yday);
	pc_check(tm->tm_wday == (int)(((days % 7) + 11) % 7), "%s: tm_wday is %d for %lld-%02d-%02d",
	    what, tm->tm_wday, y, tm->tm_mon + 1, tm->tm_mday);
}

#define NV 400
static long long vals[NV];
static int nv;

static void add(long long v)
{
	if (nv < NV && (long long)(time_t)v == v)
		vals[nv++] = v;
}

static void add_day(long long y, int m, int d)
{
	long long s = secs(y, m, d, 0, 0, 0);

	add(s);
	add(s + 12 * 3600 + 34 * 60 + 56);
	add(s + 86399);
}

/* The Epoch, leap days, ends of years and months, 2100 and 2400. */
static void collect(void)
{
	static const long long years[] = { 1970, 1971, 1972, 1999, 2000, 2001, 2038, 2099, 2100, 2101,
		2399, 2400 };
	size_t i;
	int m;

	add(0);
	add(1);
	add(951782400);		/* 2000-02-29 00:00:00 */
	add(1234567890);
	add(2147483647);
	add(2147483648LL);
	for (i = 0; i < sizeof years / sizeof years[0]; i++) {
		add_day(years[i], 1, 1);
		add_day(years[i], 2, 28);
		if (leap(years[i]))
			add_day(years[i], 2, 29);
		add_day(years[i], 3, 1);
		add_day(years[i], 12, 31);
	}
	for (m = 0; m < 36; m++)
		add_day(1999 + m / 12, m % 12 + 1, 1);
}

int main(void)
{
	char what[64];
	int i;

	pc_begin("XSH.gmtime.001");
	/* A local time 3 hours west, so that a local conversion shows. */
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	collect();
	if (sizeof(time_t) * 8 < 64)
		pc_note("time_t has fewer than 64 bits here: values it cannot hold are left out");
	pc_test();
	for (i = 0; i < nv; i++) {
		time_t t = (time_t)vals[i];
		struct tm *tm = gmtime(&t);

		snprintf(what, sizeof what, "gmtime(%lld)", vals[i]);
		pc_check(tm != NULL, "%s returned a null pointer (%s)", what, pc_errname(errno));
		check_bd(tm, vals[i], what);
		pc_check(tm->tm_isdst == 0, "%s: tm_isdst is %d", what, tm->tm_isdst);
	}
	pc_pass();
}
