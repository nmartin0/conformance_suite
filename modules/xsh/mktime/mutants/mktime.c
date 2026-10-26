/*
 * mktime.c - planted faults in mktime(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mktime.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mktime.", 7) != 0)
		return NULL;
	return m + 7;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#define MAR_2100 4107542400LL	/* 2100-03-01 00:00:00 UTC */

static char *first_tz;
static char name0[64], name1[64];

/* Run the real mktime() with TZ set to tz, then put TZ back. */
static time_t under_tz(const char *tz, struct tm *tm)
{
	REAL(mktime);
	char *now = getenv("TZ") ? strdup(getenv("TZ")) : NULL;
	time_t r;

	(void)setenv("TZ", tz, 1);
	r = real_mktime(tm);
	if (now != NULL) {
		(void)setenv("TZ", now, 1);
		free(now);
	}
	return r;
}

time_t mktime(struct tm *tm)
{
	REAL(mktime);
	REAL(localtime_r);
	const char *m;
	struct tm c;
	time_t r;
	int e;

	if (first_tz == NULL && getenv("TZ") != NULL)
		first_tz = strdup(getenv("TZ"));
	m = mutant();
	if (m == NULL)
		return real_mktime(tm);
	if (IS("utc"))
		return under_tz("UTC0", tm);	/* the timezone is ignored */
	if (IS("cached_tz") && first_tz != NULL)
		return under_tz(first_tz, tm);	/* TZ read once, at the first call */
	if (IS("keeps_tzname")) {
		snprintf(name0, sizeof name0, "%s", tzname[0] ? tzname[0] : "");
		snprintf(name1, sizeof name1, "%s", tzname[1] ? tzname[1] : "");
		r = real_mktime(tm);
		tzname[0] = name0;
		tzname[1] = name1;
		return r;
	}
	if (IS("sign")) {
		/* The offset applied the wrong way: twice the offset off. */
		r = real_mktime(tm);
		if (r != (time_t)-1) {
			r -= 2 * timezone;
			(void)real_localtime_r(&r, tm);
		}
		return r;
	}
	if (IS("uses_yday") && tm->tm_yday >= 0 && tm->tm_yday <= 365) {
		tm->tm_mon = 0;
		tm->tm_mday = tm->tm_yday + 1;
	}
	if (IS("mon_clamp"))
		tm->tm_mon = tm->tm_mon < 0 ? 0 : tm->tm_mon > 11 ? 11 : tm->tm_mon;
	if (IS("sec_clamp"))
		tm->tm_sec = tm->tm_sec < 0 ? 0 : tm->tm_sec > 60 ? 60 : tm->tm_sec;
	if (IS("isdst_ignored"))
		tm->tm_isdst = -1;
	if (IS("pos_as_zero") && tm->tm_isdst > 0)
		tm->tm_isdst = 0;
	if (IS("neg_as_zero") && tm->tm_isdst < 0)
		tm->tm_isdst = 0;
	if (IS("neg_as_pos") && tm->tm_isdst < 0)
		tm->tm_isdst = 1;
	if (IS("overflow_wraps") && tm->tm_year > INT_MAX - 400) {
		c = *tm;
		c.tm_year -= 400 * 5000000;	/* the year wrapped, no error */
		r = real_mktime(&c);
		*tm = c;
		return r;
	}
	if ((IS("gap_transition") || IS("gap_error")) && tm->tm_isdst < 0) {
		struct tm a = *tm, b = *tm, la, lb;
		time_t ra, rb;

		a.tm_isdst = 0;
		b.tm_isdst = 1;
		ra = real_mktime(&a);
		rb = real_mktime(&b);
		(void)real_localtime_r(&ra, &la);
		(void)real_localtime_r(&rb, &lb);
		/* Both readings, or neither, agree with their own tm_isdst: skipped or repeated. */
		if ((la.tm_isdst == 0) == (lb.tm_isdst > 0)) {
			if (IS("gap_error")) {
				errno = EOVERFLOW;
				return (time_t)-1;
			}
			r = ra / 2 + rb / 2;	/* the instant of the change itself */
			(void)real_localtime_r(&r, tm);
			return r;
		}
	}
	if (IS("keeps_struct") || IS("wday_kept") || IS("isdst_kept")) {
		c = *tm;
		r = real_mktime(&c);
		if (IS("wday_kept")) {
			c.tm_wday = tm->tm_wday;
			c.tm_yday = tm->tm_yday;
		}
		if (IS("isdst_kept"))
			c.tm_isdst = tm->tm_isdst;
		if (!IS("keeps_struct"))
			*tm = c;
		return r;
	}
	r = real_mktime(tm);
	e = errno;
	if (r == (time_t)-1 && e == EOVERFLOW) {
		if (IS("overflow_wday"))
			tm->tm_wday = 0;
		if (IS("overflow_errno"))
			errno = ERANGE;
	}
	if (IS("leap_2100") && r != (time_t)-1 && (long long)r >= MAR_2100) {
		r -= 86400;	/* as if 2100 had a 29 February */
		(void)real_localtime_r(&r, tm);
	}
	return r;
}
