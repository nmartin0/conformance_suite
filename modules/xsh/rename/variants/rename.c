/*
 * rename.c - conforming variants of rename() and renameat(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. A variant applies in every phase and process.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/* Does some component of p have more than max bytes? */
static int long_component(const char *p, size_t max)
{
	size_t n = 0;

	for (; *p != '\0'; p++) {
		if (*p == '/')
			n = 0;
		else if (++n > max)
			return 1;
	}
	return 0;
}

static int fail(int e)
{
	errno = e;
	return -1;
}

/*
 * "at_fdcwd_alt": AT_FDCWD is -3041965 (the header half is
 * rename.at_fdcwd_alt.h). That value means the working directory; this
 * system's own value is then an ordinary descriptor that is not open, so
 * a relative path with it fails with EBADF (the *at() ERRORS).
 * Returns the descriptor to pass on, or AT_BAD with errno set.
 */
#define ALT_FDCWD (-3041965)
#define AT_BAD (-2147483647 - 1)
static int at_fd(const char *v, int fd, const char *path)
{
	if (!IS("at_fdcwd_alt"))
		return fd;
	if (fd == ALT_FDCWD)
		return AT_FDCWD;
	if (fd == AT_FDCWD) {
		if (path[0] == '/')
			return AT_FDCWD;	/* the descriptor is not used */
		errno = EBADF;
		return AT_BAD;
	}
	return fd;
}

/* Is the directory holding the last component of path (from fd) sticky? */
static int in_sticky_dir(int fd, const char *path)
{
	char buf[4096];
	size_t n = strlen(path);
	struct stat st;
	char *s;

	if (n >= sizeof buf)
		return 0;
	memcpy(buf, path, n + 1);
	while (n > 1 && buf[n - 1] == '/')
		buf[--n] = '\0';
	s = strrchr(buf, '/');
	if (s == NULL)
		strcpy(buf, ".");
	else if (s == buf)
		buf[1] = '\0';
	else
		*s = '\0';
	return fstatat(fd, buf, &st, 0) == 0 && (st.st_mode & S_ISVTX) != 0;
}

/* Checks the variant system makes first (0: go on). */
static int before(const char *v, const char *old, const char *new)
{
	/*
	 * "name_max_14": {NAME_MAX} is 14, the least _POSIX_NAME_MAX allows
	 * (XBD <limits.h>); pathconf() says so, and a longer component gives
	 * ENAMETOOLONG (rename() ERRORS; XBD 4.16).
	 * Not declared on any test: XBD <limits.h> gives {NAME_MAX} an XSI minimum
	 * of {_XOPEN_NAME_MAX} (255) and the reference runs claim XSI, so 14
	 * models a non-XSI system. Declare it again for a profile without XSI.
	 */
	if (IS("name_max_14") && (long_component(old, 14) || long_component(new, 14)))
		return fail(ENAMETOOLONG);
	return 0;
}

/* The variant's view of a rename's result. */
static int result(const char *v, int r, int oldfd, const char *old, int newfd, const char *new)
{
	int e = errno;

	/*
	 * "sticky_eacces": a rename refused by the sticky-directory rule, for
	 * old's directory or new's, gives EACCES. rename() ERRORS (XSI) allows
	 * EPERM or EACCES for it.
	 */
	if (r == -1 && IS("sticky_eacces") && e == EPERM &&
	    (in_sticky_dir(oldfd, old) || in_sticky_dir(newfd, new)))
		return fail(EACCES);
	/*
	 * "notempty_eexist": a new name that is a directory that is not empty
	 * gives EEXIST. rename() ERRORS allows EEXIST or ENOTEMPTY.
	 */
	if (r == -1 && IS("notempty_eexist") && e == ENOTEMPTY)
		return fail(EEXIST);
	errno = e;
	return r;
}

int rename(const char *old, const char *new)
{
	const char *v = variant_of("rename.");
	REAL(rename);

	if (v == NULL)
		return real_rename(old, new);
	if (before(v, old, new) != 0)
		return -1;
	return result(v, real_rename(old, new), AT_FDCWD, old, AT_FDCWD, new);
}

int renameat(int oldfd, const char *old, int newfd, const char *new)
{
	const char *v = variant_of("rename.");
	REAL(renameat);

	if (v == NULL)
		return real_renameat(oldfd, old, newfd, new);
	if ((oldfd = at_fd(v, oldfd, old)) == AT_BAD || (newfd = at_fd(v, newfd, new)) == AT_BAD)
		return -1;
	if (before(v, old, new) != 0)
		return -1;
	return result(v, real_renameat(oldfd, old, newfd, new), oldfd, old, newfd, new);
}

/*
 * rmdir() agrees with "notempty_eexist" (rmdir() ERRORS allow EEXIST or
 * ENOTEMPTY too), so the system is consistent.
 */
int rmdir(const char *path)
{
	const char *v = variant_of("rename.");
	REAL(rmdir);

	if (v != NULL && IS("name_max_14") && long_component(path, 14))
		return fail(ENAMETOOLONG);
	if (real_rmdir(path) == 0)
		return 0;
	if (IS("notempty_eexist") && errno == ENOTEMPTY)
		errno = EEXIST;
	return -1;
}

static long limit(const char *v, int name, long real)
{
	return IS("name_max_14") && name == _PC_NAME_MAX ? 14 : real;
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("rename.");
	REAL(pathconf);

	return limit(v, name, real_pathconf(path, name));
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("rename.");
	REAL(fpathconf);

	return limit(v, name, real_fpathconf(fd, name));
}

/*
 * "coarse_times": the file system keeps timestamps in whole seconds.
 * XBD 4.12 (File Times Update) makes the resolution implementation-defined
 * but no coarser than one second, and has each timestamp set to the
 * greatest supported value not after the time it is set to: truncation.
 */
static int coarse(const char *v, int r, struct stat *st)
{
	if (r == 0 && IS("coarse_times")) {
		st->st_atim.tv_nsec = 0;
		st->st_mtim.tv_nsec = 0;
		st->st_ctim.tv_nsec = 0;
	}
	return r;
}

int stat(const char *restrict path, struct stat *restrict st)
{
	const char *v = variant_of("rename.");
	REAL(stat);

	return coarse(v, real_stat(path, st), st);
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	const char *v = variant_of("rename.");
	REAL(lstat);

	return coarse(v, real_lstat(path, st), st);
}

int fstat(int fd, struct stat *st)
{
	const char *v = variant_of("rename.");
	REAL(fstat);

	return coarse(v, real_fstat(fd, st), st);
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	const char *v = variant_of("rename.");
	REAL(fstatat);

	return coarse(v, real_fstatat(fd, path, st, flag), st);
}
