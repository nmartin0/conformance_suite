/*
 * fread.c - conforming variants of fread(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "partial_junk": when fread() stops inside an element (end-of-file or an
 * error), that element's bytes are overwritten with 0xa5. XSH fread()
 * DESCRIPTION leaves the value of a partly read element unspecified.
 */
size_t fread(void *restrict ptr, size_t size, size_t n, FILE *restrict f)
{
	const char *v = variant_of("fread.");
	off_t before = -1, after;
	size_t r;
	int e;
	REAL(fread);

	if (!IS("partial_junk") || size < 2 || n == 0)
		return real_fread(ptr, size, n, f);
	e = errno;
	before = ftello(f);
	errno = e;
	r = real_fread(ptr, size, n, f);
	if (r < n && before != -1) {
		e = errno;
		after = ftello(f);
		errno = e;
		if (after != -1 && (size_t)(after - before) % size != 0)
			memset((unsigned char *)ptr + r * size, 0xa5, size);
	}
	return r;
}
