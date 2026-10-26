/*
 * strftime() fails with ERANGE when the result and its NUL do not fit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.014
 * MUTANT    strftime.erange_einval
 * MUTANT    strftime.no_nul_room
 * VARIANT   strftime.fail_scribble
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
	struct tm tm = mk(2021, 7, 15, 12, 0, 0);
	char buf[64];
	size_t n;
	int e;

	pc_begin("XSH.strftime.014");
	pc_test();
	errno = 0;
	n = strftime(buf, 10, "%Y-%m-%d", &tm);	/* 10 bytes and the NUL */
	e = errno;
	pc_check(n == 0, "strftime() with maxsize 10 for 10 bytes returned %zu", n);
	pc_check(e == ERANGE, "strftime() with maxsize 10 for 10 bytes set errno to %s, expected ERANGE",
	    pc_errname(e));
	n = strftime(buf, 11, "%Y-%m-%d", &tm);
	pc_check(n == 10 && strcmp(buf, "2021-07-15") == 0, "strftime() with maxsize 11 returned %zu", n);
	pc_pass();
}
