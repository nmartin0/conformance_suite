/*
 * localtime() under a TZ rule of the Jn and n forms with given times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localtime.003
 * MUTANT    localtime.julian_leap
 * MUTANT    localtime.zero_based_as_julian
 * MUTANT    localtime.dst_offset_default
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

/*
 * localtime() of t must be the broken-down time whose XBD 4.19 value is
 * t + gmtoff (gmtoff: seconds east of UTC), with tm_isdst positive when
 * dst is set and 0 otherwise.
 */
static void check_local(const char *tz, long long t, long gmtoff, int dst)
{
	char what[128];
	time_t tt = (time_t)t;
	struct tm *tm;

	snprintf(what, sizeof what, "TZ=%s: localtime(%lld)", tz, t);
	errno = 0;
	tm = localtime(&tt);
	pc_check(tm != NULL, "%s returned a null pointer (%s)", what, pc_errname(errno));
	check_bd(tm, t + gmtoff, what);
	if (dst)
		pc_check(tm->tm_isdst > 0, "%s: tm_isdst is %d, expected a positive value (DST)", what,
		    tm->tm_isdst);
	else
		pc_check(tm->tm_isdst == 0, "%s: tm_isdst is %d, expected 0", what, tm->tm_isdst);
}

#define TZ_J "PCS5PCD4:30,J60/3,300/1:30"

int main(void)
{
	static const long long years[] = { 2023, 2024 };
	size_t i;

	pc_begin("XSH.localtime.003");
	if (setenv("TZ", TZ_J, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_test();
	for (i = 0; i < 2; i++) {
		long long y = years[i];
		/* J60 is 1 March in every year: 03:00 standard time, 5 h west. */
		long long start = secs(y, 3, 1, 3, 0, 0) + 18000;
		/* Day 300 from 0, leap day counted: 01:30 DST, 4 h 30 min west. */
		long long end = (days_from_civil(y, 1, 1) + 300) * 86400 + 5400 + 16200;

		check_local(TZ_J, secs(y, 2, 28, 12, 0, 0), -18000, 0);
		check_local(TZ_J, start - 1, -18000, 0);
		check_local(TZ_J, start, -16200, 1);
		check_local(TZ_J, end - 1, -16200, 1);
		check_local(TZ_J, end, -18000, 0);
	}
	/* 29 February 2024 is before J60. */
	check_local(TZ_J, secs(2024, 2, 29, 17, 0, 0), -18000, 0);
	/* 27 October: still DST in 2023 (ends on the 28th), not in 2024. */
	check_local(TZ_J, secs(2023, 10, 27, 12, 0, 0), -16200, 1);
	check_local(TZ_J, secs(2024, 10, 27, 12, 0, 0), -18000, 0);
	pc_pass();
}
