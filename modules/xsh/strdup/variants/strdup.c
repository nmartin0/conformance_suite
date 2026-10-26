/*
 * strdup.c - conforming variants of strndup(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <stdlib.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "alloc_size": strndup() allocates size + 1 bytes, not just the length
 * of the copy plus one, whenever size is below 1 MiB. APPLICATION USAGE
 * says implementations may allocate either; the bytes after the copy's
 * null byte are left as malloc() gave them (here: filled with 'Y', which
 * no caller may rely on either way). It reads no more of s than strndup()
 * itself would.
 * "n_copies_size" (a READING of XSH.strdup.007): strndup() reads all size
 * bytes of s and then cuts the copy at the first null byte. Under the
 * reading that s is an array of at least size bytes, DESCRIPTION's only
 * limit on what is examined (no more than size bytes) is kept.
 */
char *strndup(const char *s, size_t size)
{
	const char *v = variant_of("strdup.");
	REAL(strndup);
	size_t n = 0;
	char *p;

	if (IS("n_copies_size")) {
		volatile char c;
		size_t i;

		for (i = 0; i < size; i++)
			c = s[i];	/* every byte up to size is examined */
		(void)c;
		return real_strndup(s, size);
	}
	if (!IS("alloc_size") || size >= ((size_t)1 << 20))
		return real_strndup(s, size);
	while (n < size && s[n] != '\0')
		n++;
	p = malloc(size + 1);
	if (p == NULL)
		return NULL;	/* malloc() has set errno to ENOMEM */
	memset(p, 'Y', size + 1);
	memcpy(p, s, n);
	p[n] = '\0';
	return p;
}
