/*
 * ftruncate.c - conforming variants of ftruncate() and truncate(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "ebadf_einval": a descriptor open only for reading gives EBADF and one
 * that is not open gives EINVAL. ftruncate.html (ERRORS) lists "[EBADF]
 * or [EINVAL]" for any descriptor that is not open for writing, a closed
 * one included, so either error may be reported in either case.
 */
static int ebadf_einval(int fd)
{
	int fl = fcntl(fd, F_GETFL);

	if (fl == -1) {
		errno = EINVAL;
		return -1;
	}
	if ((fl & O_ACCMODE) != O_WRONLY && (fl & O_ACCMODE) != O_RDWR) {
		errno = EBADF;
		return -1;
	}
	return 0;
}

/*
 * "no_sparse": the file system has no sparse files and less than 1 GiB
 * free, so growing a file past 1 GiB fails with ENOSPC and leaves it
 * unaffected. ftruncate() lists no ENOSPC, but XSH 2.3 lets an
 * implementation report errors beyond those listed, and running out of
 * space is such a condition; a test cannot then judge a large extension.
 */
#define NO_SPARSE_MAX ((off_t)1 << 30)

static int too_big(off_t now, off_t length)
{
	return length > now && length > NO_SPARSE_MAX;
}

int ftruncate(int fd, off_t length)
{
	const char *v = variant_of("ftruncate.");
	struct stat st;
	REAL(ftruncate);

	if (IS("ebadf_einval") && ebadf_einval(fd) != 0)
		return -1;
	if (IS("no_sparse") && length >= 0 && fstat(fd, &st) == 0 && S_ISREG(st.st_mode) && too_big(st.st_size, length)) {
		errno = ENOSPC;
		return -1;
	}
	return real_ftruncate(fd, length);
}

int truncate(const char *path, off_t length)
{
	const char *v = variant_of("ftruncate.");
	struct stat st;
	REAL(truncate);

	if (IS("no_sparse") && length >= 0 && stat(path, &st) == 0 && S_ISREG(st.st_mode) && too_big(st.st_size, length)) {
		errno = ENOSPC;
		return -1;
	}
	return real_truncate(path, length);
}
