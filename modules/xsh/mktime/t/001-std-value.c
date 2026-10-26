/*
 * mktime() of in-range broken-down times under a standard-time-only TZ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mktime.001
 * MUTANT    mktime.utc
 * MUTANT    mktime.sign
 * MUTANT    mktime.leap_2100
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

int main(void)
{
	static const int d[][6] = {
		{ 1970, 1, 1, 12, 0, 0 },
		{ 2000, 2, 29, 0, 0, 0 },
		{ 2024, 2, 29, 23, 59, 59 },
		{ 2021, 10, 27, 13, 47, 51 },
		{ 2100, 3, 1, 0, 0, 0 },
		{ 2400, 2, 29, 12, 0, 0 },
	};
	size_t i;

	pc_begin("XSH.mktime.001");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_test();
	for (i = 0; i < sizeof d / sizeof d[0]; i++) {
		long long want = secs(d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5]) + 10800;
		struct tm tm = mk(d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5]);
		time_t r;

		if ((long long)(time_t)want != want) {
			pc_note("%d-%02d-%02d left out: time_t cannot hold it", d[i][0], d[i][1], d[i][2]);
			continue;
		}
		errno = 0;
		r = mktime(&tm);
		pc_check((long long)r == want, "mktime(%d-%02d-%02d %02d:%02d:%02d) returned %lld, expected %lld (%s)",
		    d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5], (long long)r, want, pc_errname(errno));
	}
	pc_pass();
}
