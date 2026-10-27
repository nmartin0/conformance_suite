/*
 * gd.h - helpers shared by the getdate tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The tests run with TZ set to a zone with no offset and no daylight saving
 * time, so that "the current local time" is simple to compute. The template
 * file is written in the test's private directory and DATEMSK is set to its
 * full pathname.
 */
#ifndef GD_H
#define GD_H

#include "pc.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GD_N(t) (sizeof(t) / sizeof((t)[0]))

static const char *const gd_wday[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
static const char *const gd_wabbr[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *const gd_mon[12] = {
	"January", "February", "March", "April", "May", "June", "July", "August",
	"September", "October", "November", "December"
};
static const char *const gd_mabbr[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* The full pathname of name in the current directory. */
static inline void gd_path(const char *name, char *out, size_t n)
{
	char cwd[512];

	pc_check(getcwd(cwd, sizeof cwd) != NULL, "getcwd failed");
	snprintf(out, n, "%s/%s", cwd, name);
}

/* Fix the zone and make the template file the content (one template per line). */
static inline void gd_setup(const char *content)
{
	char path[640];

	pc_make_file("pcg.tmpl", content);
	gd_path("pcg.tmpl", path, sizeof path);
	pc_check(setenv("DATEMSK", path, 1) == 0, "setenv DATEMSK failed");
	pc_check(setenv("TZ", "UTC0", 1) == 0, "setenv TZ failed");
	tzset();
}

/* getdate() must succeed; the result is copied before anything else can overwrite it. */
static inline struct tm gd_get(const char *s)
{
	struct tm *p = getdate(s);

	pc_check(p != NULL, "getdate(\"%s\") failed with getdate_err %d", s, (int)getdate_err);
	return *p;
}

/* Check the fields that are not negative. */
static inline void gd_fields(const struct tm *r, const char *what, int y, int m, int d,
    int H, int M, int S)
{
	pc_check(y < 0 || r->tm_year == y, "%s: tm_year is %d, expected %d", what, r->tm_year, y);
	pc_check(m < 0 || r->tm_mon == m, "%s: tm_mon is %d, expected %d", what, r->tm_mon, m);
	pc_check(d < 0 || r->tm_mday == d, "%s: tm_mday is %d, expected %d", what, r->tm_mday, d);
	pc_check(H < 0 || r->tm_hour == H, "%s: tm_hour is %d, expected %d", what, r->tm_hour, H);
	pc_check(M < 0 || r->tm_min == M, "%s: tm_min is %d, expected %d", what, r->tm_min, M);
	pc_check(S < 0 || r->tm_sec == S, "%s: tm_sec is %d, expected %d", what, r->tm_sec, S);
}

/* Is hour:minute:second of r that of the local time of some second from t1 to t2? */
static inline int gd_hms_now(const struct tm *r, time_t t1, time_t t2)
{
	time_t t;

	if (t2 < t1 || t2 - t1 > 5)
		pc_unresolved("the clock moved by %ld s during the call", (long)(t2 - t1));
	for (t = t1; t <= t2; t++) {
		struct tm *l = localtime(&t);

		if (l != NULL && l->tm_hour == r->tm_hour && l->tm_min == r->tm_min &&
		    l->tm_sec == r->tm_sec)
			return 1;
	}
	return 0;
}

static inline int gd_same_day(time_t a, time_t b)
{
	struct tm x, *p;

	p = localtime(&a);
	x = *p;
	p = localtime(&b);
	return x.tm_yday == p->tm_yday && x.tm_year == p->tm_year;
}

#endif
