/*
 * mktime() fails with EOVERFLOW, leaving tm_wday, when tm_year overflows.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mktime.009
 * MUTANT    mktime.overflow_wraps
 * MUTANT    mktime.overflow_wday
 * MUTANT    mktime.overflow_errno
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void over(int mon, int mday, int wday)
{
	struct tm tm;
	time_t r;
	int e;

	memset(&tm, 0, sizeof tm);
	tm.tm_year = INT_MAX;
	tm.tm_mon = mon;
	tm.tm_mday = mday;
	tm.tm_hour = 12;
	tm.tm_isdst = 0;
	tm.tm_wday = wday;
	errno = 0;
	r = mktime(&tm);
	e = errno;
	pc_check(r == (time_t)-1, "mktime(tm_year INT_MAX, tm_mon %d, tm_mday %d) returned %lld, expected -1", mon,
	    mday, (long long)r);
	pc_check(e == EOVERFLOW, "mktime(tm_year INT_MAX, tm_mon %d, tm_mday %d) set errno to %s, expected EOVERFLOW",
	    mon, mday, pc_errname(e));
	pc_check(tm.tm_wday == wday, "mktime(tm_year INT_MAX, tm_mon %d, tm_mday %d) changed tm_wday from %d to %d",
	    mon, mday, wday, tm.tm_wday);
}

int main(void)
{
	pc_begin("XSH.mktime.009");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	if (sizeof(time_t) * CHAR_BIT < 64)
		pc_unresolved("time_t has fewer than 64 bits: the time itself may not be representable");
	pc_test();
	over(12, 1, -1);
	over(11, 32, 7);
	pc_pass();
}
