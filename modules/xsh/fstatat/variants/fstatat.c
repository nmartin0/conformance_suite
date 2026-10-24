/*
 * fstatat.c - conforming variants of lstat() and fstatat(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <fcntl.h>
#include <sys/stat.h>
#include "pcvariant.h"

static int depth;	/* a libc that calls one of these from another: vary once */

/*
 * "link_mode": the file mode bits reported for a symbolic link are 0
 * (the reference systems report 0777). XSH fstatat() leaves them
 * unspecified for symbolic links; only the file type in st_mode must be
 * meaningful.
 */
static void cook(const char *v, struct stat *buf)
{
	if (IS("link_mode") && S_ISLNK(buf->st_mode))
		buf->st_mode &= ~(mode_t)07777;
}

int lstat(const char *restrict path, struct stat *restrict buf)
{
	const char *v = variant_of("fstatat.");
	int r;
	REAL(lstat);

	if (v == NULL || depth)
		return real_lstat(path, buf);
	depth++;
	r = real_lstat(path, buf);
	depth--;
	if (r == 0)
		cook(v, buf);
	return r;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag)
{
	const char *v = variant_of("fstatat.");
	int r;
	REAL(fstatat);

	if (v == NULL || depth)
		return real_fstatat(fd, path, buf, flag);
	depth++;
	r = real_fstatat(fd, path, buf, flag);
	depth--;
	if (r == 0)
		cook(v, buf);
	return r;
}
