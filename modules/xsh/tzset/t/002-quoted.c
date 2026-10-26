/*
 * The quoted form of std and dst in TZ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tzset.002
 * MUTANT    tzset.keeps_quotes
 * MUTANT    tzset.no_quoted
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

static void conv(const char *tz, long long t, long gmtoff, int dst)
{
	char what[96];
	time_t tt = (time_t)t;
	struct tm buf;

	snprintf(what, sizeof what, "TZ=%s: localtime_r(%lld)", tz, t);
	pc_check(localtime_r(&tt, &buf) == &buf, "%s failed: %s", what, pc_errname(errno));
	check_bd(&buf, t + gmtoff, what);
	pc_check(dst ? buf.tm_isdst > 0 : buf.tm_isdst == 0, "%s: tm_isdst is %d", what, buf.tm_isdst);
}

static void names(const char *tz, const char *n0, const char *n1)
{
	if (setenv("TZ", tz, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_check(tzname[0] != NULL && strcmp(tzname[0], n0) == 0, "TZ=%s: tzname[0] is \"%s\", expected \"%s\"", tz,
	    tzname[0] ? tzname[0] : "(null)", n0);
	if (n1 != NULL)
		pc_check(tzname[1] != NULL && strcmp(tzname[1], n1) == 0, "TZ=%s: tzname[1] is \"%s\", expected \"%s\"",
		    tz, tzname[1] ? tzname[1] : "(null)", n1);
}

int main(void)
{
	pc_begin("XSH.tzset.002");
	pc_test();
	names("<-03>3<-02>,M3.2.0,M11.1.0", "-03", "-02");
	conv("<-03>3<-02>,M3.2.0,M11.1.0", secs(2021, 1, 15, 12, 0, 0), -10800, 0);
	conv("<-03>3<-02>,M3.2.0,M11.1.0", secs(2021, 7, 15, 12, 0, 0), -7200, 1);
	names("<+0530>-5:30", "+0530", NULL);
	conv("<+0530>-5:30", secs(2021, 7, 15, 12, 0, 0), 19800, 0);
	pc_pass();
}
