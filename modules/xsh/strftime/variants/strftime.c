/*
 * strftime.c - conforming variants of strftime() and strftime_l(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <locale.h>
#include <string.h>
#include <time.h>
#include "pcvariant.h"

/*
 * "fail_scribble": when the result does not fit, the whole array of
 * maxsize bytes is filled with '#'. XSH strftime() RETURN VALUE leaves the
 * contents of the array unspecified when 0 is returned for a failure; the
 * DESCRIPTION still keeps every byte within the first maxsize.
 */
static char probe[8192];

/* Fill the array when n == 0 was a failure (the result is not empty). */
static void scribble(char *s, size_t max, size_t n, size_t probed)
{
	if (n == 0 && probed > 0 && max > 0)
		memset(s, '#', max);
}

size_t strftime(char *restrict s, size_t max, const char *restrict fmt, const struct tm *restrict tm)
{
	const char *v = variant_of("strftime.");
	REAL(strftime);
	size_t n = real_strftime(s, max, fmt, tm);

	if (IS("fail_scribble") && n == 0)
		scribble(s, max, n, real_strftime(probe, sizeof probe, fmt, tm));
	return n;
}

size_t strftime_l(char *restrict s, size_t max, const char *restrict fmt, const struct tm *restrict tm,
    locale_t loc)
{
	const char *v = variant_of("strftime.");
	REAL(strftime_l);
	size_t n = real_strftime_l(s, max, fmt, tm, loc);

	if (IS("fail_scribble") && n == 0)
		scribble(s, max, n, real_strftime_l(probe, sizeof probe, fmt, tm, loc));
	return n;
}
