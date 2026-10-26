/*
 * symlink.c - conforming variants of symlink() and symlinkat(), and of the file system
 * behaviour they show, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int fail(int e)
{
	errno = e;
	return -1;
}

/* The last component of p, trailing slashes removed, into buf ("" if none). */
static const char *last_of(const char *p, char *buf, size_t size)
{
	size_t n = strlen(p), s;

	while (n > 1 && p[n - 1] == '/')
		n--;
	s = n;
	while (s > 0 && p[s - 1] != '/')
		s--;
	if (n - s >= size)
		n = s + size - 1;
	memcpy(buf, p + s, n - s);
	buf[n - s] = '\0';
	return buf;
}

/* The parent directory of p (trailing slashes ignored) into buf. */
static const char *parent_of(const char *p, char *buf, size_t size)
{
	size_t n = strlen(p);

	while (n > 1 && p[n - 1] == '/')
		n--;
	while (n > 0 && p[n - 1] != '/')
		n--;
	while (n > 1 && p[n - 1] == '/')
		n--;
	if (n == 0)
		return ".";
	if (n >= size)
		n = size - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';
	return buf;
}

/*
 * "eilseq": the file system holds only portable filenames (XBD 3: the
 * portable filename character set) and refuses any other name. XSH
 * symlink() ERRORS: EILSEQ is the error for a last component that is not a
 * portable filename and cannot be created in the target directory.
 */
static int portable(const char *name)
{
	const char *c;

	for (c = name; *c != '\0'; c++)
		if (!((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9') ||
		      *c == '.' || *c == '_' || *c == '-'))
			return 0;
	return 1;
}

/*
 * "slash_enotdir": a name with a trailing slash is refused with ENOTDIR
 * before anything else is looked at. XSH symlink() ERRORS allows ENOENT or
 * ENOTDIR for such a name, and rules out only ENOENT when the name without
 * the slashes exists.
 */
static int has_slash_end(const char *p)
{
	size_t n = strlen(p);

	return n > 1 && p[n - 1] == '/';
}

/*
 * "parent_gid": the new file takes the group of the directory it is made
 * in, which XSH symlink() DESCRIPTION allows (the parent's group or the
 * effective group ID).
 */
static void take_parent_gid(int fd, const char *path)
{
	char b[PATH_MAX];
	struct stat p;
	int saved = errno;
	REAL(fstatat);

	if (real_fstatat(fd, parent_of(path, b, sizeof b), &p, 0) == 0)
		(void)fchownat(fd, path, (uid_t)-1, p.st_gid, AT_SYMLINK_NOFOLLOW);
	errno = saved;
}

static int before(const char *v, const char *path)
{
	char b[PATH_MAX];

	if (IS("slash_enotdir") && has_slash_end(path))
		return fail(ENOTDIR);
	if (IS("eilseq") && !portable(last_of(path, b, sizeof b)))
		return fail(EILSEQ);
	return 0;
}

/*
 * "symlink_max_255": {SYMLINK_MAX} is 255, the least value XBD <limits.h>
 * allows (_POSIX_SYMLINK_MAX), and symlink() refuses a longer path1 with
 * ENAMETOOLONG (XSH symlink() ERRORS); pathconf() and fpathconf() report
 * the limit.
 */
#define SYMLINK_MIN 255

static int make(int fd, const char *path1, const char *path2)
{
	const char *v = variant_of("symlink.");
	int r;
	REAL(symlinkat);

	if (v != NULL && before(v, path2) != 0)
		return -1;
	if (IS("symlink_max_255") && strlen(path1) > SYMLINK_MIN)
		return fail(ENAMETOOLONG);
	r = real_symlinkat(path1, fd, path2);
	if (r == 0 && IS("parent_gid"))
		take_parent_gid(fd, path2);
	return r;
}

int symlink(const char *path1, const char *path2)
{
	return make(AT_FDCWD, path1, path2);
}

int symlinkat(const char *path1, int fd, const char *path2)
{
	return make(fd, path1, path2);
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("symlink.");
	REAL(pathconf);

	if (IS("symlink_max_255") && name == _PC_SYMLINK_MAX)
		return SYMLINK_MIN;
	return real_pathconf(path, name);
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("symlink.");
	REAL(fpathconf);

	if (IS("symlink_max_255") && name == _PC_SYMLINK_MAX)
		return SYMLINK_MIN;
	return real_fpathconf(fd, name);
}

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; a time set to V is recorded as the
 * greatest supported value not above V, so the stat functions report whole
 * seconds.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("symlink.");

	if (r == 0 && IS("coarse_times")) {
		st->st_atim.tv_nsec = 0;
		st->st_mtim.tv_nsec = 0;
		st->st_ctim.tv_nsec = 0;
	}
	return r;
}

int stat(const char *restrict path, struct stat *restrict st)
{
	REAL(stat);
	return coarse(real_stat(path, st), st);
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	REAL(lstat);
	return coarse(real_lstat(path, st), st);
}

int fstat(int fd, struct stat *st)
{
	REAL(fstat);
	return coarse(real_fstat(fd, st), st);
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	REAL(fstatat);
	return coarse(real_fstatat(fd, path, st, flag), st);
}
