/*
 * tzset.c - planted faults in tzset(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "tzset.<fault>" and apply only while
 * PC_PHASE is "test".
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

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "tzset.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static char b0[64], b1[64];
static int calls;
static char *last_tz;	/* the TZ of the last tzset() that took effect */

void tzset(void)
{
	REAL(tzset);
	const char *m = mutant();
	const char *tz = getenv("TZ");

	if (IS("noop") || IS("stale")) {
		if (calls++ > 0)
			return;	/* TZ read once, never again */
	}
	if (IS("no_quoted") && tz != NULL && tz[0] == '<') {
		/* The quoted form not recognised: TZ taken as invalid, UTC used. */
		char *keep = strdup(tz);

		(void)setenv("TZ", "UTC0", 1);
		real_tzset();
		if (keep != NULL) {
			(void)setenv("TZ", keep, 1);
			free(keep);
		}
		return;
	}
	real_tzset();
	free(last_tz);
	last_tz = tz ? strdup(tz) : NULL;
	if (IS("swap")) {
		char *t = tzname[0];

		tzname[0] = tzname[1];
		tzname[1] = t;
	}
	if (IS("trunc3")) {
		snprintf(b0, 4, "%s", tzname[0] ? tzname[0] : "");
		snprintf(b1, 4, "%s", tzname[1] ? tzname[1] : "");
		tzname[0] = b0;
		tzname[1] = b1;
	}
	if (IS("keeps_quotes") && tz != NULL && tz[0] == '<') {
		snprintf(b0, sizeof b0, "<%s>", tzname[0] ? tzname[0] : "");
		snprintf(b1, sizeof b1, "<%s>", tzname[1] ? tzname[1] : "");
		tzname[0] = b0;
		tzname[1] = b1;
	}
	if (IS("daylight_always"))
		daylight = 1;
	if (IS("timezone_sign"))
		timezone = -timezone;
	if (IS("timezone_dst") && daylight)
		timezone -= 3600;	/* the DST offset in place of the standard one */
	if (IS("timezone_no_secs"))
		timezone = timezone / 60 * 60;
}

/* With "stale", conversions keep using the TZ of the first tzset(). */
struct tm *localtime_r(const time_t *restrict t, struct tm *restrict res)
{
	REAL(localtime_r);
	const char *m = mutant();
	char *now;
	struct tm *r;

	if (!IS("stale") || last_tz == NULL)
		return real_localtime_r(t, res);
	now = getenv("TZ") ? strdup(getenv("TZ")) : NULL;
	(void)setenv("TZ", last_tz, 1);
	r = real_localtime_r(t, res);
	if (now != NULL) {
		(void)setenv("TZ", now, 1);
		free(now);
	}
	return r;
}
