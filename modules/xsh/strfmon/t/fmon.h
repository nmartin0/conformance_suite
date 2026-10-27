/*
 * fmon.h - helpers for the strfmon tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef FMON_H
#define FMON_H

#include "lc.h"

static locale_t fm_obj;		/* (locale_t)0: strfmon(); else strfmon_l() with this object */

static inline ssize_t fm_one(char *buf, size_t n, const char *fmt, double v)
{
	return fm_obj == (locale_t)0 ? strfmon(buf, n, fmt, v) : strfmon_l(buf, n, fm_obj, fmt, v);
}

/* The digits of s, in order, with everything else left out. */
static inline void fm_digits(const char *s, char *d, size_t n)
{
	size_t k = 0;

	for (; *s != '\0' && k + 1 < n; s++)
		if (*s >= '0' && *s <= '9')
			d[k++] = *s;
	d[k] = '\0';
}

/* Format one value; the call must succeed. */
static inline void fm_ok(char *buf, size_t n, const char *fmt, double v)
{
	ssize_t r = fm_one(buf, n, fmt, v);

	pc_check(r >= 0, "strfmon(\"%s\", %g) failed, errno %s", fmt, v, pc_errname(errno));
	pc_check((size_t)r == strlen(buf), "strfmon(\"%s\", %g) returned %ld but the result \"%s\" has length %zu", fmt, v, (long)r, buf, strlen(buf));
}

/* Format one value and check that its digits are exactly want. */
static inline void fm_want(const char *fmt, double v, const char *want)
{
	char buf[128], dg[128];

	fm_ok(buf, sizeof buf, fmt, v);
	fm_digits(buf, dg, sizeof dg);
	pc_check(strcmp(dg, want) == 0, "strfmon(\"%s\", %g) gave \"%s\" with digits \"%s\", expected digits \"%s\"", fmt, v, buf, dg, want);
}

#endif
