/*
 * puts.c - conforming variants of puts(), for the variant checks (CI
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
 * "returns_count": a successful puts() returns the number of bytes it
 * wrote, the newline included ({INT_MAX} for longer strings).
 * "returns_last": it returns the last byte written, the newline. XSH
 * puts(), RETURN VALUE asks only for a non-negative value on success; its
 * APPLICATION USAGE names both conventions.
 */
int puts(const char *s)
{
	const char *v = variant_of("puts.");
	REAL(puts);
	int r = real_puts(s);
	size_t l;

	if (v == NULL || r == EOF)
		return r;
	l = strlen(s);
	if (IS("returns_count"))
		return l >= INT_MAX ? INT_MAX : (int)l + 1;
	if (IS("returns_last"))
		return '\n';
	return r;
}
