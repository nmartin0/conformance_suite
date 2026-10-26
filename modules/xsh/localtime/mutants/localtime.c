/*
 * localtime.c - planted faults in localtime() and localtime_r(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "localtime.<fault>" and apply only
 * while PC_PHASE is "test". Faults in the reading of a TZ rule are planted
 * by handing the real localtime() a TZ value rewritten the way a faulty
 * parser would read it, then putting TZ back.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "localtime.", 10) != 0)
		return NULL;
	return m + 10;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#define YEARS_400 12622780800LL	/* seconds in 400 Gregorian years */

/* TZ rewritten: the first "from" replaced by "to". 1 if TZ was changed. */
static char *saved_tz;

static int tz_swap(const char *from, const char *to)
{
	const char *tz = getenv("TZ");
	const char *p;
	char buf[512];

	if (tz == NULL || (p = strstr(tz, from)) == NULL)
		return 0;
	snprintf(buf, sizeof buf, "%.*s%s%s", (int)(p - tz), tz, to, p + strlen(from));
	saved_tz = strdup(tz);
	if (saved_tz == NULL)
		return 0;
	(void)setenv("TZ", buf, 1);
	return 1;
}

static void tz_back(int swapped)
{
	if (!swapped)
		return;
	(void)setenv("TZ", saved_tz, 1);
	free(saved_tz);
	saved_tz = NULL;
}

/* The faulty readings of a TZ rule, as rewrites of the TZ value. */
static int tz_fault(const char *m)
{
	if (IS("no_seconds"))
		return tz_swap(":15:30", ":15");		/* the :ss part dropped */
	if (IS("default_time_0"))
		return tz_swap("M3.2.0,M11.1.0", "M3.2.0/0,M11.1.0/0");	/* default time 00:00 */
	if (IS("julian_leap"))
		return tz_swap(",J60/", ",59/");		/* Jn counting 29 February */
	if (IS("zero_based_as_julian"))
		return tz_swap(",300/", ",J300/");		/* n read as Jn */
	if (IS("dst_offset_default"))
		return tz_swap("PCD4:30,", "PCD,");		/* explicit dst offset ignored */
	if (IS("no_wrap"))
		return tz_swap("PCW-10PCX,M10.1.0,M4.1.0/3", "PCW-10");	/* end before start: no DST */
	if (IS("neg_time"))
		return tz_swap("/-1,", ",");			/* negative time ignored */
	if (IS("time_clamp"))
		return tz_swap("/26", "/24") || tz_swap("/25", "/24");	/* hours clamped to 24 */
	return 0;
}

/* The XBD 4.19 value of a broken-down time (year >= 1970). */
static long long value(const struct tm *tm)
{
	long long y = tm->tm_year;

	return tm->tm_sec + tm->tm_min * 60LL + tm->tm_hour * 3600LL + tm->tm_yday * 86400LL +
	    (y - 70) * 31536000LL + ((y - 69) / 4) * 86400LL - ((y - 1) / 100) * 86400LL +
	    ((y + 299) / 400) * 86400LL;
}

static struct tm own;

/* tzname kept by copying: the pointers may lead to buffers that are reused. */
static char name0[64], name1[64];

static void keep_names(void)
{
	snprintf(name0, sizeof name0, "%s", tzname[0] ? tzname[0] : "");
	snprintf(name1, sizeof name1, "%s", tzname[1] ? tzname[1] : "");
}

static void put_names(void)
{
	tzname[0] = name0;
	tzname[1] = name1;
}
static char *first_tz;

/* Standard time in place of DST: the time "timezone" seconds west. */
static struct tm *as_std(const time_t *t, struct tm *r)
{
	REAL(gmtime_r);
	time_t u;

	if (r == NULL || r->tm_isdst <= 0)
		return r;
	u = *t - timezone;
	real_gmtime_r(&u, r);
	r->tm_isdst = 0;
	return r;
}

struct tm *localtime(const time_t *t)
{
	REAL(localtime);
	REAL(gmtime);
	REAL(gmtime_r);
	const char *m;
	struct tm *r;
	int sw;

	if (first_tz == NULL && getenv("TZ") != NULL)
		first_tz = strdup(getenv("TZ"));
	m = mutant();
	if (m == NULL)
		return real_localtime(t);
	if (IS("utc"))
		return real_gmtime(t);
	if (IS("overflow_wraps") && (long long)*t > 1000LL * YEARS_400) {
		time_t u = (time_t)((long long)*t % YEARS_400);
		return real_localtime(&u);
	}
	if (IS("cached_tz") && first_tz != NULL) {
		char *now = getenv("TZ") ? strdup(getenv("TZ")) : NULL;

		(void)setenv("TZ", first_tz, 1);
		r = real_localtime(t);
		if (now != NULL) {
			(void)setenv("TZ", now, 1);
			free(now);
		}
		return r;
	}
	if (IS("keeps_tzname")) {
		keep_names();
		r = real_localtime(t);
		put_names();
		return r;
	}
	sw = tz_fault(m);
	r = real_localtime(t);
	tz_back(sw);
	if (r == NULL) {
		if (IS("overflow_errno"))
			errno = ERANGE;
		return r;
	}
	if (IS("sign")) {
		long long off = value(r) - (long long)*t;
		time_t u = (time_t)((long long)*t - off);
		int dst = r->tm_isdst;

		r = real_gmtime_r(&u, &own);
		r->tm_isdst = dst;
	}
	if (IS("no_dst"))
		r = as_std(t, r);
	if (IS("gmtoff_sign"))
		r->tm_gmtoff = -r->tm_gmtoff;
	if (IS("zone_std"))
		r->tm_zone = tzname[0];
	return r;
}

struct tm *localtime_r(const time_t *restrict t, struct tm *restrict res)
{
	REAL(localtime_r);
	REAL(localtime);
	REAL(gmtime_r);
	REAL(tzset);
	const char *m = mutant();
	struct tm *r;

	if (m == NULL)
		return real_localtime_r(t, res);
	if (IS("r_static"))
		return real_localtime(t);
	if (IS("r_utc"))
		return real_gmtime_r(t, res);
	if (IS("r_overflow_wraps") && (long long)*t > 1000LL * YEARS_400) {
		time_t u = (time_t)((long long)*t % YEARS_400);
		return real_localtime_r(&u, res);
	}
	if (IS("r_tzname_only")) {
		int d = daylight;
		long z = timezone;

		real_tzset();
		r = real_localtime_r(t, res);
		daylight = d;
		timezone = z;
		return r;
	}
	if (IS("r_daylight_only")) {
		long z = timezone;

		keep_names();
		real_tzset();
		r = real_localtime_r(t, res);
		put_names();
		timezone = z;
		return r;
	}
	r = real_localtime_r(t, res);
	if (r == NULL) {
		if (IS("overflow_errno"))
			errno = ERANGE;
		return r;
	}
	if (IS("r_no_dst"))
		r = as_std(t, r);
	if (IS("gmtoff_sign"))
		r->tm_gmtoff = -r->tm_gmtoff;
	if (IS("zone_std"))
		r->tm_zone = tzname[0];
	return r;
}
