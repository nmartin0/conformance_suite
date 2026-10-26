/*
 * rewind.c - conforming variants of rewind(), for the variant checks (CI
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
 * "lseek_first": before it writes any pending output, rewind() asks the
 * descriptor for its offset; if that fails, it gives up at once with
 * lseek()'s error in errno, writes nothing, and clears the error indicator
 * as it always does. rewind()'s errors are fseek()'s, and XSH 2.3 leaves
 * the order in which errors are detected open and lets a failed call have
 * done only part of its work. On a pipe this gives ESPIPE before any EPIPE
 * or EAGAIN could arise; on a closed descriptor, EBADF. The failed seek
 * leaves the end-of-file indicator as it was.
 */

/*
 * The error indicator alone is cleared behind the C library's back: both
 * reference C libraries keep it in the first word of the FILE object
 * (glibc's _IO_ERR_SEEN, musl's F_ERR, both 0x20).
 */
static void clear_error_only(FILE *f)
{
	*(unsigned *)(void *)f &= ~0x20u;
}

void rewind(FILE *f)
{
	const char *v = variant_of("rewind.");
	int e, fd;
	REAL(rewind);

	if (IS("lseek_first")) {
		e = errno;
		fd = fileno(f);
		errno = e;
		if (fd >= 0 && lseek(fd, 0, SEEK_CUR) == -1) {
			clear_error_only(f);
			return;
		}
	}
	real_rewind(f);
}
