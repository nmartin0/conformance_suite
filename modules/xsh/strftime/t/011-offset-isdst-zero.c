/*
 * %z with tm_isdst 0 on a DST structure: either offset, in the +hhmm form.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.011
 * MUTANT    strftime.z_sign
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


int main(void)
{
	time_t t = (time_t)secs(2021, 7, 15, 16, 0, 0);
	struct tm jul, *p;

	pc_begin("XSH.strftime.011");
	if (setenv("TZ", "PCS5PCD,M3.2.0,M11.1.0", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	p = localtime(&t);
	pc_check(p != NULL && p->tm_isdst > 0, "localtime() did not apply the DST rule");
	jul = *p;
	jul.tm_isdst = 0;
	pc_test();
	{
		char buf[64];
		size_t n = strftime(buf, sizeof buf, "%z", &jul);

		/* The standard offset (tm_isdst) or the DST one (tm_gmtoff): the text allows either reading. */
		pc_check((n == 5 && strcmp(buf, "-0500") == 0) || (n == 5 && strcmp(buf, "-0400") == 0),
		    "strftime(\"%%z\") gave \"%.*s\" (return %zu), expected \"-0500\" or \"-0400\"",
		    (int)(n < sizeof buf ? n : 0), buf, n);
	}
	pc_pass();
}
