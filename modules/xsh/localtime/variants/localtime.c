/*
 * localtime.c - conforming variants of localtime_r(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <time.h>
#include "pcvariant.h"

/*
 * localtime() DESCRIPTION: unlike localtime(), localtime_r() need not set
 * tzname; if it does, it sets daylight and timezone too, and if it does
 * not, it sets neither. Both are allowed:
 * "r_sets_tz": localtime_r() sets the timezone information as tzset()
 * does, from the TZ of the moment, before converting.
 * "r_keeps_tz": localtime_r() leaves tzname, daylight and timezone as
 * they were (whatever it uses for the conversion itself).
 */
struct tm *localtime_r(const time_t *restrict t, struct tm *restrict res)
{
	const char *v = variant_of("localtime.");
	REAL(localtime_r);
	REAL(tzset);
	struct tm *r;

	if (IS("r_sets_tz")) {
		real_tzset();
		return real_localtime_r(t, res);
	}
	if (IS("r_keeps_tz")) {
		/* The names are copied: the pointers may lead to reused buffers. */
		static char n0[64], n1[64];
		int d = daylight;
		long z = timezone;

		snprintf(n0, sizeof n0, "%s", tzname[0] ? tzname[0] : "");
		snprintf(n1, sizeof n1, "%s", tzname[1] ? tzname[1] : "");
		r = real_localtime_r(t, res);
		tzname[0] = n0;
		tzname[1] = n1;
		daylight = d;
		timezone = z;
		return r;
	}
	return real_localtime_r(t, res);
}
