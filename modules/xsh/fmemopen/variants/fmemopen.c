/*
 * fmemopen.c - conforming variants of fmemopen(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdio.h>
#include "pcvariant.h"

/*
 * "size0_einval": a buffer size of zero is not supported, and fmemopen()
 * fails with EINVAL for it. The fmemopen() page lists exactly this as a
 * "may fail" error (and FUTURE DIRECTIONS only considers requiring zero
 * sizes later); other sizes work as usual.
 *
 * "unbuffered": memory streams start unbuffered, so every write reaches
 * the memory buffer at once. Neither the fmemopen() page nor XSH 2.5 fixes
 * the initial buffering of a memory stream (2.5 describes what happens if
 * it is fully or line buffered), and setvbuf() can still change it.
 */
FILE *fmemopen(void *restrict buf, size_t size, const char *restrict mode)
{
	const char *v = variant_of("fmemopen.");
	REAL(fmemopen);
	REAL(setvbuf);
	FILE *f;
	int e;

	if (IS("size0_einval") && size == 0) {
		errno = EINVAL;
		return NULL;
	}
	f = real_fmemopen(buf, size, mode);
	if (IS("unbuffered") && f != NULL) {
		e = errno;
		(void)real_setvbuf(f, NULL, _IONBF, 0);
		errno = e;
	}
	return f;
}
