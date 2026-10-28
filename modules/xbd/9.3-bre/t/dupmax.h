/*
 * dupmax.h - interval expressions up to {RE_DUP_MAX}, for a BRE or an ERE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef DUPMAX_H
#define DUPMAX_H

#include "rx.h"
#include <errno.h>
#include <limits.h>
#include <unistd.h>

static inline long dupmax_limit(void)
{
	long v;

#ifdef RE_DUP_MAX
	v = RE_DUP_MAX;
#else
	errno = 0;
	v = sysconf(_SC_RE_DUP_MAX);
	if (v < 0)
		pc_notinuse("{RE_DUP_MAX} has no definite value");
#endif
	if (v > 65536)
		v = 65536;
	return v;
}

/* The pattern is searched in a string of len letters a. */
static inline void dupmax_try(const char *pat, int cf, size_t len, int so, int eo)
{
	char *str = malloc(len + 1);
	regex_t probe;
	int r;

	if (str == NULL)
		pc_unresolved("out of memory");
	r = regcomp(&probe, pat, cf);
	if (r == REG_ESPACE)
		pc_unresolved("regcomp ran out of memory on a count of %zu", len);
	if (r == 0)
		regfree(&probe);
	memset(str, 'a', len);
	str[len] = '\0';
	rx_one(pat, cf, str, so, eo);
	free(str);
}

static inline void dupmax_run(int cf)
{
	long m = dupmax_limit();
	size_t n = (size_t)m;
	char pat[64];
	const char *o = (cf & REG_EXTENDED) ? "{" : "\\{";
	const char *c = (cf & REG_EXTENDED) ? "}" : "\\}";

	snprintf(pat, sizeof pat, "^a%s%ld%s$", o, m, c);
	dupmax_try(pat, cf, n, 0, (int)(n));
	dupmax_try(pat, cf, n - 1, -1, 0);
	dupmax_try(pat, cf, n + 1, -1, 0);

	snprintf(pat, sizeof pat, "^a%s0,%ld%s$", o, m, c);
	dupmax_try(pat, cf, n, 0, (int)(n));
	dupmax_try(pat, cf, 0, 0, (int)(0));
	dupmax_try(pat, cf, n + 1, -1, 0);

	snprintf(pat, sizeof pat, "^a%s%ld,%s$", o, m, c);
	dupmax_try(pat, cf, n, 0, (int)(n));
	dupmax_try(pat, cf, n + 1, 0, (int)(n + 1));
	dupmax_try(pat, cf, n - 1, -1, 0);

	snprintf(pat, sizeof pat, "a%s%ld%s", o, m, c);
	dupmax_try(pat, cf, n + 1, 0, (int)n);
}

#endif
