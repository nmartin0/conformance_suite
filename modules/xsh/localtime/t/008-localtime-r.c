/*
 * localtime_r() stores what localtime() returns, and returns result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localtime.008
 * MUTANT    localtime.r_static
 * MUTANT    localtime.r_utc
 * MUTANT    localtime.r_no_dst
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

/* Day of the month of the n'th weekday d (0 = Sunday) of month m (1..12); n 5 is the last. */
static int mday_of(long long y, int m, int n, int d)
{
	int w1 = (int)(((days_from_civil(y, m, 1) % 7) + 11) % 7);
	int day = 1 + (d - w1 + 7) % 7 + 7 * (n - 1);

	while (day > mdays(y, m - 1))
		day -= 7;
	return day;
}

#define TZ_D "PCS5PCD,M3.2.0,M11.1.0"

static void same(long long v, const struct tm *a, const struct tm *b)
{
	pc_check(a->tm_sec == b->tm_sec && a->tm_min == b->tm_min && a->tm_hour == b->tm_hour &&
	    a->tm_mday == b->tm_mday && a->tm_mon == b->tm_mon && a->tm_year == b->tm_year &&
	    a->tm_wday == b->tm_wday && a->tm_yday == b->tm_yday && (a->tm_isdst > 0) == (b->tm_isdst > 0) &&
	    (a->tm_isdst == 0) == (b->tm_isdst == 0),
	    "for %lld localtime_r() gave %d-%02d-%02d %02d:%02d:%02d isdst %d, localtime() %d-%02d-%02d %02d:%02d:%02d isdst %d",
	    v, a->tm_year + 1900, a->tm_mon + 1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec, a->tm_isdst,
	    b->tm_year + 1900, b->tm_mon + 1, b->tm_mday, b->tm_hour, b->tm_min, b->tm_sec, b->tm_isdst);
}

int main(void)
{
	long long start, end, vals[8];
	size_t i, n = 0;

	pc_begin("XSH.localtime.008");
	if (setenv("TZ", TZ_D, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	start = secs(2021, 3, mday_of(2021, 3, 2, 0), 2, 0, 0) + 18000;
	end = secs(2021, 11, mday_of(2021, 11, 1, 0), 2, 0, 0) + 14400;
	vals[n++] = secs(2021, 1, 15, 12, 0, 0);
	vals[n++] = secs(2021, 7, 15, 12, 0, 0);
	vals[n++] = start - 1;
	vals[n++] = start;
	vals[n++] = end - 1;
	vals[n++] = end;
	pc_test();
	for (i = 0; i < n; i++) {
		time_t t = (time_t)vals[i];
		struct tm buf, ref, *r, *p;

		memset(&buf, 0x5a, sizeof buf);
		r = localtime_r(&t, &buf);
		pc_check(r == &buf, "localtime_r(%lld) returned %p, not result (%p)", vals[i], (void *)r,
		    (void *)&buf);
		p = localtime(&t);
		pc_check(p != NULL, "localtime(%lld) returned a null pointer", vals[i]);
		ref = *p;
		same(vals[i], &buf, &ref);
		/* And the UTC instant itself: the offset of the rule. */
		check_bd(&buf, vals[i] - ((buf.tm_isdst > 0) ? 14400 : 18000), "localtime_r()");
		pc_check((buf.tm_isdst > 0) == (vals[i] >= start && vals[i] < end),
		    "localtime_r(%lld): tm_isdst %d on the wrong side of a change", vals[i], buf.tm_isdst);
	}
	pc_pass();
}
