/*
 * strnlen.c - conforming variants (and readings) of strnlen(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <string.h>
#include "pcvariant.h"

/*
 * "scans_max" (a READING of XSH.strnlen.005): strnlen() reads all maxlen
 * bytes and then reports the position of the first null byte. Under the
 * reading that s is an array of at least maxlen bytes, DESCRIPTION's only
 * limit on what is examined (never more than maxlen bytes) is kept, and
 * the result is the one RETURN VALUE requires.
 */
size_t strnlen(const char *s, size_t max)
{
	const char *v = variant_of("strnlen.");
	REAL(strnlen);
	volatile char c;
	size_t i, first = max;

	if (!IS("scans_max"))
		return real_strnlen(s, max);
	for (i = 0; i < max; i++) {
		c = s[i];
		if (c == '\0' && first == max)
			first = i;
	}
	return first;
}
