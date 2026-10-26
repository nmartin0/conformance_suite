/*
 * mktime() carries out-of-range members as its steps describe.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mktime.003
 * MUTANT    mktime.mon_clamp
 * MUTANT    mktime.sec_clamp
 * MUTANT    mktime.keeps_struct
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

/* The carried value: months move the year, the rest is linear. */
static long long carried(int y, int mon, int mday, int h, int mi, int s)
{
	long long yy = y + (mon >= 0 ? mon / 12 : -((11 - mon) / 12));
	int mm = mon - (int)(yy - y) * 12;

	return (days_from_civil(yy, mm + 1, 1) + mday - 1) * 86400 + h * 3600LL + mi * 60LL + s;
}

int main(void)
{
	/* year, tm_mon (0..11 is in range), tm_mday, hour, minute, second; the expected date */
	static const struct {
		int in[6];
		int want[6];
	} c[] = {
		{ { 2021, 1, 29, 12, 0, 0 }, { 2021, 3, 1, 12, 0, 0 } },
		{ { 2021, 1, 0, 12, 0, 0 }, { 2021, 1, 31, 12, 0, 0 } },
		{ { 2021, 5, 10, 21, 65, 0 }, { 2021, 6, 10, 22, 5, 0 } },
		{ { 2021, -1, 15, 12, 0, 0 }, { 2020, 12, 15, 12, 0, 0 } },
		{ { 2021, 12, 15, 12, 0, 0 }, { 2022, 1, 15, 12, 0, 0 } },
		{ { 2021, 25, 15, 12, 0, 0 }, { 2023, 2, 15, 12, 0, 0 } },
		{ { 2020, 2, 0, 12, 0, 0 }, { 2020, 2, 29, 12, 0, 0 } },
		{ { 1999, 13, 29, 12, 0, 0 }, { 2000, 2, 29, 12, 0, 0 } },
		{ { 2000, 13, 29, 12, 0, 0 }, { 2001, 3, 1, 12, 0, 0 } },
		{ { 2001, 0, 366, 12, 0, 0 }, { 2002, 1, 1, 12, 0, 0 } },
		{ { 2021, 2, -30, 12, 0, 0 }, { 2021, 1, 29, 12, 0, 0 } },
		{ { 2021, 5, 10, -1, 0, 0 }, { 2021, 6, 9, 23, 0, 0 } },
		{ { 2021, 5, 10, 48, 0, 0 }, { 2021, 6, 12, 0, 0, 0 } },
		{ { 2021, 5, 10, 0, -1, 0 }, { 2021, 6, 9, 23, 59, 0 } },
		{ { 2021, 0, 1, 0, 0, -1 }, { 2020, 12, 31, 23, 59, 59 } },
		{ { 2021, 5, 10, 12, 0, 60 }, { 2021, 6, 10, 12, 1, 0 } },
		{ { 2021, 5, 10, 12, 0, 3600 }, { 2021, 6, 10, 13, 0, 0 } },
		{ { 2021, 11, 31, 23, 59, 60 }, { 2022, 1, 1, 0, 0, 0 } },
	};
	char what[128];
	size_t i;

	pc_begin("XSH.mktime.003");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	/* The carried values of the table agree with the expected dates. */
	for (i = 0; i < sizeof c / sizeof c[0]; i++)
		pc_check(carried(c[i].in[0], c[i].in[1], c[i].in[2], c[i].in[3], c[i].in[4], c[i].in[5]) ==
		    secs(c[i].want[0], c[i].want[1], c[i].want[2], c[i].want[3], c[i].want[4], c[i].want[5]),
		    "test table row %zu is inconsistent", i);
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		const int *a = c[i].in;
		long long local = secs(c[i].want[0], c[i].want[1], c[i].want[2], c[i].want[3], c[i].want[4],
		    c[i].want[5]);
		struct tm tm;
		time_t r;

		memset(&tm, 0, sizeof tm);
		tm.tm_year = a[0] - 1900;
		tm.tm_mon = a[1];
		tm.tm_mday = a[2];
		tm.tm_hour = a[3];
		tm.tm_min = a[4];
		tm.tm_sec = a[5];
		tm.tm_isdst = 0;
		snprintf(what, sizeof what, "mktime(year %d mon %d mday %d %d:%d:%d)", a[0], a[1], a[2], a[3], a[4], a[5]);
		r = mktime(&tm);
		pc_check((long long)r == local + 10800, "%s returned %lld, expected %lld (%d-%02d-%02d %02d:%02d:%02d)",
		    what, (long long)r, local + 10800, c[i].want[0], c[i].want[1], c[i].want[2], c[i].want[3],
		    c[i].want[4], c[i].want[5]);
		check_bd(&tm, local, what);
		pc_check(tm.tm_isdst == 0, "%s: tm_isdst is %d afterwards", what, tm.tm_isdst);
	}
	pc_pass();
}
