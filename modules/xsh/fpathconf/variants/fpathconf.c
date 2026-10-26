/*
 * fpathconf.c - conforming variants of pathconf() and fpathconf(), for
 * the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "unassociated": wherever a Requirements note of the fpathconf() table
 * leaves it unspecified whether a name is associated with a file of that
 * type, this system does not support the association and fails with
 * EINVAL (the may-fail [EINVAL] of ERRORS): the terminal variables (note
 * 2) on non-terminals, the note 4 names on non-directories, {PIPE_BUF} on
 * files other than directories and FIFOs (note 6), the note 8 names on
 * directories, and the note 10 names on files other than regular files.
 */
static int unassociated(int name, const struct stat *st)
{
	switch (name) {
	case _PC_MAX_CANON: case _PC_MAX_INPUT: case _PC_VDISABLE:
		return !S_ISCHR(st->st_mode);
	case _PC_FILESIZEBITS: case _PC_NAME_MAX: case _PC_PATH_MAX: case _PC_2_SYMLINKS:
	case _PC_SYMLINK_MAX: case _PC_NO_TRUNC:
#ifdef _PC_TEXTDOMAIN_MAX
	case _PC_TEXTDOMAIN_MAX:
#endif
		return !S_ISDIR(st->st_mode);
	case _PC_PIPE_BUF:
		return !S_ISDIR(st->st_mode) && !S_ISFIFO(st->st_mode);
	case _PC_ASYNC_IO: case _PC_PRIO_IO: case _PC_SYNC_IO:
#ifdef _PC_FALLOC
	case _PC_FALLOC:
#endif
		return S_ISDIR(st->st_mode);
	case _PC_ALLOC_SIZE_MIN: case _PC_REC_INCR_XFER_SIZE: case _PC_REC_MAX_XFER_SIZE:
	case _PC_REC_MIN_XFER_SIZE: case _PC_REC_XFER_ALIGN:
		return !S_ISREG(st->st_mode);
	}
	return 0;
}

/*
 * "no_limit": {LINK_MAX} and {SYMLINK_MAX} have no limit on any file:
 * -1 with errno unchanged, as RETURN VALUE allows for a <limits.h>
 * maximum with no limit. Used only where this system's <limits.h> does
 * not define the constant, so compile time and run time agree.
 */
static int no_limit(int name)
{
	switch (name) {
#ifndef LINK_MAX
	case _PC_LINK_MAX:
#endif
#ifndef SYMLINK_MAX
	case _PC_SYMLINK_MAX:
#endif
		return 1;
	}
	return 0;
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("fpathconf.");
	struct stat st;
	int e = errno, have;
	REAL(pathconf);

	if (v == NULL)
		return real_pathconf(path, name);
	have = stat(path, &st) == 0;
	errno = e;
	if (IS("unassociated") && have && unassociated(name, &st)) {
		errno = EINVAL;
		return -1;
	}
	if (IS("no_limit") && no_limit(name))
		return -1;
	return real_pathconf(path, name);
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("fpathconf.");
	struct stat st;
	int e = errno, have;
	REAL(fpathconf);

	if (v == NULL)
		return real_fpathconf(fd, name);
	have = fstat(fd, &st) == 0;
	errno = e;
	if (IS("unassociated") && have && unassociated(name, &st)) {
		errno = EINVAL;
		return -1;
	}
	if (IS("no_limit") && no_limit(name))
		return -1;
	return real_fpathconf(fd, name);
}
