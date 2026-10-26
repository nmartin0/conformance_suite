/*
 * gmtime.c - planted faults in gmtime() and gmtime_r(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "gmtime.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "gmtime.", 7) != 0)
		return NULL;
	return m + 7;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#define MAR_2100 4107542400LL	/* 2100-03-01 00:00:00 UTC */
#define YEARS_400 12622780800LL	/* seconds in 400 Gregorian years */

/* As if 2100 were a leap year: from 1 March 2100 every date is a day early. */
static void leap_2100(time_t t, struct tm *tm, struct tm *(*conv)(const time_t *, struct tm *))
{
	time_t u;

	if ((long long)t < MAR_2100)
		return;
	u = t - 86400;
	conv(&u, tm);
	if (tm->tm_year == 200 && tm->tm_mon == 1 && tm->tm_mday == 28) {
		tm->tm_mday = 29;
		tm->tm_yday++;
	}
	tm->tm_wday = (tm->tm_wday + 1) % 7;
}

struct tm *gmtime(const time_t *t)
{
	REAL(gmtime);
	REAL(gmtime_r);
	REAL(localtime);
	const char *m = mutant();
	struct tm *r;

	if (IS("local"))
		return real_localtime(t);
	if (IS("overflow_wraps") && (long long)*t > 1000LL * YEARS_400) {
		time_t u = (time_t)((long long)*t % YEARS_400);
		return real_gmtime(&u);	/* silently wraps into 400 years */
	}
	if (IS("overflow_errno")) {
		r = real_gmtime(t);
		if (r == NULL)
			errno = ERANGE;
		return r;
	}
	r = real_gmtime(t);
	if (r == NULL)
		return r;
	if (IS("leap_2100"))
		leap_2100(*t, r, real_gmtime_r);
	if (IS("wday"))
		r->tm_wday = (r->tm_wday + 1) % 7;
	if (IS("isdst"))
		r->tm_isdst = -1;
	return r;
}

static char zbuf[16];

void tzset(void)
{
	REAL(tzset);
	const char *m = mutant();

	real_tzset();
	if (IS("r_zone_buffer"))
		strcpy(zbuf, "PC?");	/* the buffer tm_zone points at is reused */
}

struct tm *gmtime_r(const time_t *restrict t, struct tm *restrict res)
{
	REAL(gmtime_r);
	REAL(gmtime);
	REAL(localtime_r);
	const char *m = mutant();
	struct tm *r;

	if (IS("r_static"))
		return real_gmtime(t);
	if (IS("r_local"))
		return real_localtime_r(t, res);
	if (IS("r_overflow_wraps") && (long long)*t > 1000LL * YEARS_400) {
		time_t u = (time_t)((long long)*t % YEARS_400);
		return real_gmtime_r(&u, res);
	}
	r = real_gmtime_r(t, res);
	if (IS("overflow_errno") && r == NULL)
		errno = ERANGE;
	if (r == NULL)
		return r;
	if (IS("r_leap_2100"))
		leap_2100(*t, r, real_gmtime_r);
	if (IS("r_zone_gmt"))
		r->tm_zone = "GMT";
	if (IS("r_zone_buffer")) {
		strcpy(zbuf, "UTC");
		r->tm_zone = zbuf;
	}
	if (IS("r_gmtoff"))
		r->tm_gmtoff = -10800;	/* the local offset copied in */
	return r;
}
