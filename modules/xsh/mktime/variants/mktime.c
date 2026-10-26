/*
 * mktime.c - conforming variants of mktime(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <time.h>
#include "pcvariant.h"

/*
 * "gap_other": for a local time that is skipped or repeated at a change
 * to or from DST, with a negative tm_isdst, mktime() takes the reading
 * the real one does not. XSH mktime() DESCRIPTION leaves unspecified
 * whether the result is that of a positive or of a zero tm_isdst there;
 * the members are then set from localtime() of the value, as required.
 */
time_t mktime(struct tm *tm)
{
	const char *v = variant_of("mktime.");
	REAL(mktime);
	REAL(localtime_r);
	struct tm a, b, la, lb, in;
	time_t ra, rb, r;

	if (!IS("gap_other") || tm->tm_isdst >= 0)
		return real_mktime(tm);
	in = *tm;
	a = in;
	b = in;
	a.tm_isdst = 0;
	b.tm_isdst = 1;
	ra = real_mktime(&a);
	rb = real_mktime(&b);
	r = real_mktime(tm);
	if (ra == (time_t)-1 || rb == (time_t)-1 || real_localtime_r(&ra, &la) == NULL ||
	    real_localtime_r(&rb, &lb) == NULL)
		return r;
	/* Both readings, or neither, agree with their own tm_isdst: skipped or repeated. */
	if ((la.tm_isdst == 0) != (lb.tm_isdst > 0) || ra == rb)
		return r;
	r = r == ra ? rb : ra;
	(void)real_localtime_r(&r, tm);
	return r;
}
