/*
 * tzh.h - helpers for the TZ tests: calendar arithmetic and conversion of
 * a time under a given TZ value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef TZH_H
#define TZH_H

#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Days from 1970-01-01 to y-m-d in the proleptic Gregorian calendar (m 1..12). */
static long long tz_days(long long y, int m, int d)
{
	long long era, yoe, doy, doe;

	y -= m <= 2;
	era = (y >= 0 ? y : y - 399) / 400;
	yoe = y - era * 400;
	doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
	doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
	return era * 146097 + doe - 719468;
}

/* Seconds since the epoch of a UTC date and time. */
static long long tz_utc(int y, int mo, int d, int h, int mi, int s)
{
	return tz_days(y, mo, d) * 86400 + h * 3600 + mi * 60 + s;
}

static void tz_set(const char *tz)
{
	if (setenv("TZ", tz, 1) != 0)
		pc_unresolved("setenv TZ failed");
	tzset();
}

/* Convert t under the current TZ; the local calendar fields are reduced to seconds on the same scale as t. */
static int tz_conv(long long t, long long *local, int *isdst)
{
	struct tm tmv;
	time_t tt = (time_t)t;

	if (localtime_r(&tt, &tmv) == NULL)
		return -1;
	*local = tz_days(tmv.tm_year + 1900LL, tmv.tm_mon + 1, tmv.tm_mday) * 86400 + tmv.tm_hour * 3600 + tmv.tm_min * 60 + tmv.tm_sec;
	*isdst = tmv.tm_isdst;
	return 0;
}

/*
 * Check that under tz the instant t is on the given side of a change:
 * expected offset east of UTC (seconds) and DST flag (0 or non-zero).
 */
static void tz_expect(const char *tz, long long t, long long east, int dst, const char *what)
{
	long long local;
	int isdst;

	pc_check(tz_conv(t, &local, &isdst) == 0, "%s: localtime_r failed for TZ=%s", what, tz);
	pc_check(local - t == east, "%s: TZ=%s gives an offset of %lld seconds east, not %lld", what, tz, local - t, east);
	pc_check((isdst != 0) == (dst != 0), "%s: TZ=%s gives tm_isdst %d, not %s", what, tz, isdst, dst ? "positive" : "0");
}

#endif
