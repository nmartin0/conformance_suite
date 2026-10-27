/*
 * sp.h - helpers for the strptime tests: run one conversion and compare
 * the returned position and the tm members the page says it sets.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Name conversions are always followed in the input by a delimiter that
 * the format also has (or by the end of the string), and numeric
 * conversions by a delimiter or by a full-width field, so the tests do not
 * depend on how far a conversion scans before it compares.
 */
#ifndef SP_H
#define SP_H

#include "pc.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SP_UNSET (-7777)

#define SP_SEC 1
#define SP_MIN 2
#define SP_HOUR 4
#define SP_MDAY 8
#define SP_MON 16
#define SP_YEAR 32
#define SP_WDAY 64
#define SP_YDAY 128
#define SP_ISDST 256

/* A tm whose members hold a value no conversion produces. */
static inline struct tm sp_blank(void)
{
	struct tm tm;

	memset(&tm, 0, sizeof tm);
	tm.tm_sec = tm.tm_min = tm.tm_hour = tm.tm_mday = tm.tm_mon = SP_UNSET;
	tm.tm_year = tm.tm_wday = tm.tm_yday = tm.tm_isdst = SP_UNSET;
	return tm;
}

static inline void sp_cmp(const char *buf, const char *fmt, const char *name, int got, int want)
{
	pc_check(got == want, "strptime(\"%s\", \"%s\"): %s is %d, expected %d", buf, fmt, name, got, want);
}

/*
 * strptime(buf, fmt, &tm) must succeed and return buf + off; each member
 * selected in mask must equal that member of want.
 */
static inline void sp_case(const char *buf, const char *fmt, long off, unsigned mask, struct tm want)
{
	struct tm tm = sp_blank();
	char *r = strptime(buf, fmt, &tm);

	pc_check(r != NULL, "strptime(\"%s\", \"%s\") returned a null pointer", buf, fmt);
	pc_check(r == buf + off, "strptime(\"%s\", \"%s\") returned offset %ld, expected %ld",
	    buf, fmt, (long)(r - buf), off);
	if (mask & SP_SEC)
		sp_cmp(buf, fmt, "tm_sec", tm.tm_sec, want.tm_sec);
	if (mask & SP_MIN)
		sp_cmp(buf, fmt, "tm_min", tm.tm_min, want.tm_min);
	if (mask & SP_HOUR)
		sp_cmp(buf, fmt, "tm_hour", tm.tm_hour, want.tm_hour);
	if (mask & SP_MDAY)
		sp_cmp(buf, fmt, "tm_mday", tm.tm_mday, want.tm_mday);
	if (mask & SP_MON)
		sp_cmp(buf, fmt, "tm_mon", tm.tm_mon, want.tm_mon);
	if (mask & SP_YEAR)
		sp_cmp(buf, fmt, "tm_year", tm.tm_year, want.tm_year);
	if (mask & SP_WDAY)
		sp_cmp(buf, fmt, "tm_wday", tm.tm_wday, want.tm_wday);
	if (mask & SP_YDAY)
		sp_cmp(buf, fmt, "tm_yday", tm.tm_yday, want.tm_yday);
	if (mask & SP_ISDST)
		sp_cmp(buf, fmt, "tm_isdst", tm.tm_isdst, want.tm_isdst);
}

/* Success and position only (the effect on tm is unspecified or not wanted). */
static inline void sp_pos(const char *buf, const char *fmt, long off)
{
	sp_case(buf, fmt, off, 0, (struct tm){ 0 });
}

/* strptime must return a null pointer. */
static inline void sp_null(const char *buf, const char *fmt)
{
	struct tm tm = sp_blank();

	pc_check(strptime(buf, fmt, &tm) == NULL, "strptime(\"%s\", \"%s\") did not return a null pointer", buf, fmt);
}

#endif
