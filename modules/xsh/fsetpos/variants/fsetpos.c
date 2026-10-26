/*
 * fsetpos.c - conforming variants of fsetpos(), for the variant checks
 * (CI only).
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
 * "lseek_first": before it writes any pending output, fsetpos() asks the
 * descriptor for its offset; if that fails, fsetpos() fails with lseek()'s
 * error at once and writes nothing. XSH 2.3 leaves the order in which
 * errors are detected open and lets a failed call have done only part of
 * its work. No write error has then happened, and fsetpos() sets the error
 * indicator only for a read or write error (XSH fsetpos(), DESCRIPTION),
 * so the indicator stays clear. On a closed descriptor this gives EBADF.
 */
int fsetpos(FILE *f, const fpos_t *pos)
{
	const char *v = variant_of("fsetpos.");
	int e, fd;
	REAL(fsetpos);

	if (IS("lseek_first")) {
		e = errno;
		fd = fileno(f);
		errno = e;
		if (fd >= 0 && lseek(fd, 0, SEEK_CUR) == -1)
			return -1;
	}
	return real_fsetpos(f, pos);
}
