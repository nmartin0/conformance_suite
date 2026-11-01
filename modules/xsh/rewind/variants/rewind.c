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
 * The error indicator alone is cleared: the stream goes on reporting
 * end-of-file, which a small table that feof() and clearerr() consult
 * keeps for it.
 */
static FILE *eof_kept[4];

int feof(FILE *f)
{
	REAL(feof);
	int i;

	for (i = 0; i < 4; i++)
		if (eof_kept[i] == f)
			return 1;
	return real_feof(f);
}

void clearerr(FILE *f)
{
	REAL(clearerr);
	int i;

	for (i = 0; i < 4; i++)
		if (eof_kept[i] == f)
			eof_kept[i] = NULL;
	real_clearerr(f);
}

static void clear_error_only(FILE *f)
{
	int i, at_eof = feof(f);

	clearerr(f);
	for (i = 0; at_eof && i < 4; i++)
		if (eof_kept[i] == NULL) {
			eof_kept[i] = f;
			break;
		}
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
