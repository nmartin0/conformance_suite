/*
 * strtol.c - conforming variants of strtol() and strtoll(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "nc_einval": a call that performs no conversion (and has a supported
 * base) also sets errno to EINVAL. The RETURN VALUE and ERRORS sections
 * let an implementation report EINVAL when no conversion could be
 * performed; it is only a "may". Nothing else changes.
 */
/*
 * "base37_means_37" (a reading of a disputed assertion, not a variant of a
 * settled one): a base the page never defines subject sequences for is
 * given a meaning, digits 0-9 and a-z (case-insensitively) with their
 * values, so base 37 is supported and EINVAL does not apply. The page
 * requires EINVAL only for a base that is "not supported" and does not say
 * which bases those are.
 */
static long long conv37(const char *s, char **e, long long mx, long long mn)
{
	const char *p = s;
	int neg = 0, n = 0;
	unsigned long long acc = 0;

	while (*p == ' ' || (*p >= '\t' && *p <= '\r'))
		p++;
	if (*p == '+' || *p == '-')
		neg = *p++ == '-';
	for (;; p++, n++) {
		int c = (unsigned char)*p, d;
		if (c >= '0' && c <= '9')
			d = c - '0';
		else if (c >= 'a' && c <= 'z')
			d = c - 'a' + 10;
		else if (c >= 'A' && c <= 'Z')
			d = c - 'A' + 10;
		else
			break;
		acc = acc * 37 + (unsigned long long)d;
	}
	if (e != NULL)
		*e = (char *)(n ? p : s);
	if (neg)
		return acc > (unsigned long long)mx + 1 ? mn : (long long)(0ULL - acc);
	return acc > (unsigned long long)mx ? mx : (long long)acc;
}

#define VWRAP(id, sym, symfb, T, UT) \
T id(const char *restrict s, char **restrict e, int b) __asm__(sym); \
T id(const char *restrict s, char **restrict e, int b) \
{ \
	static T (*real)(const char *restrict, char **restrict, int); \
	const char *v = variant_of("strtol."); \
	char *loc; \
	T r; \
	if (real == NULL) \
		real = (T (*)(const char *restrict, char **restrict, int))dlsym(RTLD_NEXT, sym); \
	if (real == NULL) \
		real = (T (*)(const char *restrict, char **restrict, int))dlsym(RTLD_NEXT, symfb); \
	if (v == NULL) \
		return real(s, e, b); \
	if (IS("base37_means_37") && b == 37) \
		return (T)conv37(s, e, (long long)((UT)-1 >> 1), -(long long)((UT)-1 >> 1) - 1); \
	r = real(s, &loc, b); \
	if (e != NULL) \
		*e = loc; \
	if (IS("nc_einval") && loc == s && (b == 0 || (b >= 2 && b <= 36))) \
		errno = EINVAL; \
	return r; \
}

/* defined under their symbol names: a header may redirect the plain names */
VWRAP(f_strtol, "strtol", "strtol", long, unsigned long)
VWRAP(f_strtoll, "strtoll", "strtoll", long long, unsigned long long)
VWRAP(f_isoc23_strtol, "__isoc23_strtol", "strtol", long, unsigned long)
VWRAP(f_isoc23_strtoll, "__isoc23_strtoll", "strtoll", long long, unsigned long long)
