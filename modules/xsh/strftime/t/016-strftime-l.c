/*
 * strftime_l() with a POSIX locale object is strftime() in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.016
 * MUTANT    strftime.l_counts_nul
 * MUTANT    strftime.l_abday_full
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
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

static struct tm local(long long t)
{
	time_t tt = (time_t)t;
	struct tm *p = localtime(&tt);

	if (p == NULL)
		pc_unresolved("localtime(%lld) failed: %s", t, pc_errname(errno));
	return *p;
}

#define ALL "%a %A %b %B %c %C %d %D %e %F %g %G %h %H %I %j %m %M %n %p %r %R %s %S %t %T %u %U %V %w %W %x %X %y %Y %z %Z %%"

int main(void)
{
	struct tm dates[3];
	char a[512], b[512];
	locale_t loc;
	size_t i, na, nb;

	pc_begin("XSH.strftime.016");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	loc = newlocale(LC_TIME_MASK, "POSIX", (locale_t)0);
	pc_check(loc != (locale_t)0, "newlocale(LC_TIME_MASK, \"POSIX\") failed: %s", pc_errname(errno));
	/* From localtime(), so that %Z and %z have their members set. */
	dates[0] = local(secs(2021, 1, 2, 3, 4, 5));
	dates[1] = local(secs(2021, 12, 25, 15, 30, 0));
	dates[2] = local(secs(1999, 1, 2, 23, 59, 59));
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(a, 1, sizeof a);
		memset(b, 2, sizeof b);
		na = strftime_l(a, sizeof a, ALL, &dates[i], loc);
		nb = strftime(b, sizeof b, ALL, &dates[i]);
		pc_check(nb > 0 && nb < sizeof b, "strftime() returned %zu", nb);
		pc_check(na == nb && memcmp(a, b, nb + 1) == 0,
		    "strftime_l() gave \"%.*s\" (return %zu), strftime() \"%s\" (return %zu)",
		    (int)(na < sizeof a ? na : 0), a, na, b, nb);
	}
	freelocale(loc);
	pc_pass();
}
