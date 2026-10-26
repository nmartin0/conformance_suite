/*
 * freopen.c - conforming variants of freopen(), for the variant checks (CI
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
 * "null_refused": freopen() with a null pathname permits no change of mode
 * at all: it closes the stream and fails with EBADF. XSH freopen()
 * DESCRIPTION makes it implementation-defined which changes are permitted
 * "(if any)", and the original stream is closed whether or not the call
 * succeeds; EBADF is the "may fail" error for a mode the descriptor does
 * not support.
 */
FILE *freopen(const char *restrict path, const char *restrict mode, FILE *restrict f)
{
	const char *v = variant_of("freopen.");
	REAL(freopen);

	if (IS("null_refused") && path == NULL) {
		(void)fclose(f);
		errno = EBADF;
		return NULL;
	}
	return real_freopen(path, mode, f);
}
