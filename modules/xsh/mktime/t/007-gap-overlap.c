/*
 * mktime() of skipped and repeated local times with a negative tm_isdst.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mktime.007
 * MUTANT    mktime.gap_transition
 * MUTANT    mktime.gap_error
 * VARIANT   mktime.gap_other
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

#define TZ_D "PCS5PCD,M3.2.0,M11.1.0"

/*
 * Local d h:mi of 2021 with tm_isdst -1 must give std (the standard time
 * reading) or dst (the DST reading); change is the UTC instant of the
 * change, before which DST is in effect when dst_before is set.
 */
static void either(int m, int d, int h, int mi, long long change, int dst_before)
{
	long long std = secs(2021, m, d, h, mi, 0) + 18000, dst = secs(2021, m, d, h, mi, 0) + 14400;
	char what[128];
	struct tm tm;
	time_t r;
	int indst;

	memset(&tm, 0, sizeof tm);
	tm.tm_year = 121;
	tm.tm_mon = m - 1;
	tm.tm_mday = d;
	tm.tm_hour = h;
	tm.tm_min = mi;
	tm.tm_isdst = -1;
	snprintf(what, sizeof what, "TZ=%s: mktime(2021-%02d-%02d %02d:%02d, tm_isdst -1)", TZ_D, m, d, h, mi);
	errno = 0;
	r = mktime(&tm);
	pc_check((long long)r == std || (long long)r == dst,
	    "%s returned %lld, expected %lld (standard time) or %lld (DST) (%s)", what, (long long)r, std, dst,
	    pc_errname(errno));
	pc_note("%s: the %s reading", what, (long long)r == std ? "standard time" : "DST");
	indst = ((long long)r < change) == dst_before;
	check_bd(&tm, (long long)r - (indst ? 14400 : 18000), what);
	if (indst)
		pc_check(tm.tm_isdst > 0, "%s: tm_isdst is %d afterwards, expected positive", what, tm.tm_isdst);
	else
		pc_check(tm.tm_isdst == 0, "%s: tm_isdst is %d afterwards, expected 0", what, tm.tm_isdst);
}

int main(void)
{
	pc_begin("XSH.mktime.007");
	if (setenv("TZ", TZ_D, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_test();
	/* 02:00 to 03:00 on 14 March is skipped; the change is at 07:00 UTC. */
	either(3, 14, 2, 30, secs(2021, 3, 14, 7, 0, 0), 0);
	/* 01:00 to 02:00 on 7 November is repeated; the change is at 06:00 UTC. */
	either(11, 7, 1, 30, secs(2021, 11, 7, 6, 0, 0), 1);
	pc_pass();
}
