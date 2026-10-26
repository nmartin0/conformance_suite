/*
 * localtime() and localtime_r() set tm_gmtoff and tm_zone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localtime.012
 * MUTANT    localtime.gmtoff_sign
 * MUTANT    localtime.zone_std
 * VARIANT   tzset.tm_members
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

#define TZ_D "PCS5PCD,M3.2.0,M11.1.0"

static void check_members(const char *what, const struct tm *tm, long gmtoff, const char *zone)
{
	pc_check(tm->tm_gmtoff == gmtoff, "%s: tm_gmtoff is %ld, expected %ld", what, (long)tm->tm_gmtoff, gmtoff);
	pc_check(tm->tm_zone != NULL && strcmp(tm->tm_zone, zone) == 0, "%s: tm_zone is \"%s\", expected \"%s\"",
	    what, tm->tm_zone ? tm->tm_zone : "(null)", zone);
}

int main(void)
{
	time_t jan = (time_t)secs(2021, 1, 15, 12, 0, 0), jul = (time_t)secs(2021, 7, 15, 12, 0, 0);
	struct tm buf, *r;

	pc_begin("XSH.localtime.012");
	if (setenv("TZ", TZ_D, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_test();
	r = localtime(&jan);
	pc_check(r != NULL, "localtime() returned a null pointer");
	check_members("localtime(January)", r, -18000, "PCS");
	r = localtime(&jul);
	pc_check(r != NULL, "localtime() returned a null pointer");
	check_members("localtime(July)", r, -14400, "PCD");
	pc_check(localtime_r(&jan, &buf) == &buf, "localtime_r() failed");
	check_members("localtime_r(January)", &buf, -18000, "PCS");
	pc_check(localtime_r(&jul, &buf) == &buf, "localtime_r() failed");
	check_members("localtime_r(July)", &buf, -14400, "PCD");
	pc_pass();
}
