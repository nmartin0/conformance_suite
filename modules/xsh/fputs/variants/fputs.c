/*
 * fputs.c - conforming variants of fputs(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "returns_count": a successful fputs() returns the number of bytes it
 * wrote ({INT_MAX} for longer strings). "returns_last": it returns the
 * last byte written (0 for an empty string, where none is). XSH fputs(),
 * RETURN VALUE asks only for a non-negative value on success; its
 * APPLICATION USAGE names both conventions.
 */
int fputs(const char *restrict s, FILE *restrict f)
{
	const char *v = variant_of("fputs.");
	REAL(fputs);
	int r = real_fputs(s, f);
	size_t l;

	if (v == NULL || r == EOF)
		return r;
	l = strlen(s);
	if (IS("returns_count"))
		return l > INT_MAX ? INT_MAX : (int)l;
	if (IS("returns_last"))
		return l > 0 ? (unsigned char)s[l - 1] : 0;
	return r;
}
