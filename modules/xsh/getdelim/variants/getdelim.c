/*
 * getdelim.c - conforming variants of getdelim() and getline(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "pcvariant.h"

/*
 * "n_exact": an object that the call allocated or reallocated is made
 * exactly large enough for the record and its terminating null byte, and
 * *n is that size. XSH getdelim(), DESCRIPTION asks for an object large
 * enough for what is written to it and for *n to be its new size; it does
 * not say how much larger it may be.
 */
static void exact(char **lp, size_t *np, ssize_t r, char *before, size_t old)
{
	char *q;

	if (r < 0 || (*lp == before && *np == old))
		return;
	q = realloc(*lp, (size_t)r + 1);
	if (q != NULL) {
		*lp = q;
		*np = (size_t)r + 1;
	}
}

ssize_t getdelim(char **restrict lp, size_t *restrict np, int delim, FILE *restrict f)
{
	const char *v = variant_of("getdelim.");
	REAL(getdelim);
	char *before = lp != NULL ? *lp : NULL;
	size_t old = np != NULL ? *np : 0;
	ssize_t r = real_getdelim(lp, np, delim, f);

	if (IS("n_exact") && lp != NULL && np != NULL)
		exact(lp, np, r, before, old);
	return r;
}

ssize_t getline(char **restrict lp, size_t *restrict np, FILE *restrict f)
{
	const char *v = variant_of("getdelim.");
	REAL(getline);
	char *before = lp != NULL ? *lp : NULL;
	size_t old = np != NULL ? *np : 0;
	ssize_t r = real_getline(lp, np, f);

	if (IS("n_exact") && lp != NULL && np != NULL)
		exact(lp, np, r, before, old);
	return r;
}
