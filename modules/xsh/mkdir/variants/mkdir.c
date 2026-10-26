/*
 * mkdir.c - conforming variants of mkdir() and mkdirat(), and of the file
 * system behaviour they show, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

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
 * "eilseq": the file system holds only portable filenames (XBD 3.265: the
 * portable filename character set) and refuses any other name. XSH mkdir()
 * ERRORS: EILSEQ is the error for a last component that is not a portable
 * filename and that cannot be created in the target directory.
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
 * "link_max_min": {LINK_MAX} is 8, the least _POSIX_LINK_MAX allows (XBD
 * <limits.h>), and a directory's link count counts its subdirectories, so
 * mkdir() fails with EMLINK (XSH mkdir() ERRORS) once the parent has 8.
 */
#define LINK_MIN 8

static int before(const char *v, int dirfd, const char *path)
{
	char b[PATH_MAX];
	struct stat st;
	REAL(fstatat);

	if (IS("eilseq") && !portable(last_of(path, b, sizeof b)))
		return fail(EILSEQ);
	if (IS("link_max_min") && real_fstatat(dirfd, parent_of(path, b, sizeof b), &st, 0) == 0 &&
	    S_ISDIR(st.st_mode) && st.st_nlink >= LINK_MIN && fstatat(dirfd, path, &st, AT_SYMLINK_NOFOLLOW) != 0)
		return fail(EMLINK);
	return 0;
}

/*
 * "parent_gid": the new directory takes the parent directory's group, as
 * XSH mkdir() DESCRIPTION allows (parent's group or the effective group).
 * "sgid_bit": as "parent_gid", and every new directory also has S_ISGID
 * set. mkdir() initializes only the permission bits and S_ISVTX from mode
 * (XSH mkdir() DESCRIPTION); nothing requires S_ISGID of a new directory
 * to be clear, and its meaning on a directory is not specified (XBD <sys/stat.h> gives S_ISGID only for execution). This variant does it everywhere.
 */
static void after(const char *v, int dirfd, const char *path)
{
	char b[PATH_MAX];
	struct stat p, st;
	int saved = errno;
	REAL(fstatat);

	snprintf(b, sizeof b, "%s/..", path);
	if (real_fstatat(dirfd, b, &p, 0) == 0)
		(void)fchownat(dirfd, path, (uid_t)-1, p.st_gid, 0);
	if (IS("sgid_bit") && real_fstatat(dirfd, path, &st, 0) == 0)
		(void)fchmodat(dirfd, path, (st.st_mode & 07777) | S_ISGID, 0);
	errno = saved;
}

static int make(int dirfd, const char *path, mode_t mode)
{
	const char *v = variant_of("mkdir.");
	int r;
	REAL(mkdirat);

	if (v != NULL && before(v, dirfd, path) != 0)
		return -1;
	r = real_mkdirat(dirfd, path, mode);
	if (r == 0 && (IS("parent_gid") || IS("sgid_bit")))
		after(v, dirfd, path);
	return r;
}

int mkdir(const char *path, mode_t mode)
{
	return make(AT_FDCWD, path, mode);
}

int mkdirat(int dirfd, const char *path, mode_t mode)
{
	return make(dirfd, path, mode);
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("mkdir.");
	REAL(pathconf);

	if (IS("link_max_min") && name == _PC_LINK_MAX)
		return LINK_MIN;
	return real_pathconf(path, name);
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("mkdir.");
	REAL(fpathconf);

	if (IS("link_max_min") && name == _PC_LINK_MAX)
		return LINK_MIN;
	return real_fpathconf(fd, name);
}

/*
 * "no_dots": directories have no entries for dot and dot-dot. An empty
 * directory holds at most those two (XBD 3 Definitions, Empty Directory),
 * and readdir() returns them only where they exist (XSH readdir()).
 */
struct dirent *readdir(DIR *d)
{
	const char *v = variant_of("mkdir.");
	struct dirent *e;
	REAL(readdir);

	do
		e = real_readdir(d);
	while (e != NULL && IS("no_dots") && (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0));
	return e;
}

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; a time set to V is recorded as the
 * greatest supported value not above V, so stat() reports whole seconds.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("mkdir.");

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
