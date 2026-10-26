/*
 * rmdir.c - conforming variants of rmdir(), for the variant checks (CI
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
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

static int fail(int e)
{
	errno = e;
	return -1;
}

/* The last component of p, trailing slashes removed, into buf. */
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

/* 1 if the directory path names is this process's working directory or open in it. */
static int in_use(const char *path)
{
	struct stat d, o;
	int fd;
	REAL(lstat);
	REAL(fstat);

	if (real_lstat(path, &d) != 0 || !S_ISDIR(d.st_mode))
		return 0;
	if (real_lstat(".", &o) == 0 && o.st_dev == d.st_dev && o.st_ino == d.st_ino)
		return 1;
	for (fd = 0; fd < 1024; fd++)
		if (real_fstat(fd, &o) == 0 && o.st_dev == d.st_dev && o.st_ino == d.st_ino)
			return 1;
	return 0;
}

int rmdir(const char *path)
{
	const char *v = variant_of("rmdir.");
	char b[PATH_MAX];
	struct stat st;
	int r;
	REAL(rmdir);
	REAL(lstat);

	if (v == NULL)
		return real_rmdir(path);
	/*
	 * "dotdot_einval": a final dot-dot fails with EINVAL, as for a final
	 * dot. XSH rmdir() requires such a call to fail and XSH 2.3 lets a
	 * listed error be reported in other circumstances.
	 */
	if (IS("dotdot_einval") && strcmp(last_of(path, b, sizeof b), "..") == 0)
		return fail(EINVAL);
	/*
	 * "ebusy_open": a directory that is this process's working directory or
	 * open in it is in use, and this implementation counts that as an error
	 * (XSH rmdir() ERRORS, EBUSY; DESCRIPTION for the working directory).
	 */
	if (IS("ebusy_open") && in_use(path))
		return fail(EBUSY);
	r = real_rmdir(path);
	if (r == 0)
		return r;
	/* "eexist": a directory that is not empty gives EEXIST, which XSH rmdir() allows as well as ENOTEMPTY. */
	if (IS("eexist") && errno == ENOTEMPTY)
		errno = EEXIST;
	/* "sticky_eacces": the XBD 4.5 refusal in a S_ISVTX directory is EACCES, which XSH rmdir() allows as well as EPERM. */
	if (IS("sticky_eacces") && errno == EPERM)
		errno = EACCES;
	/*
	 * Not declared by any test: the other readings of disputed assertions
	 * (a disputed test logs INSPECT for them by design). For a manual run.
	 * "enoent_nondir" (Q-44): a path naming a non-directory (not a
	 * symbolic link) names a nonexistent directory, ENOENT.
	 * "enoent_lookup" (Q-8): a lookup that cannot find the last
	 * component reports ENOENT before ENAMETOOLONG or ELOOP.
	 */
	if (IS("enoent_nondir") && errno == ENOTDIR &&
	    !(real_lstat(path, &st) == 0 && S_ISLNK(st.st_mode) && path[strlen(path) - 1] != '/'))
		errno = ENOENT;
	if (IS("enoent_lookup") && (errno == ENAMETOOLONG || errno == ELOOP))
		errno = ENOENT;
	return r;
}

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; a time set to V is recorded as the
 * greatest supported value not above V, so stat() reports whole seconds.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("rmdir.");

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
