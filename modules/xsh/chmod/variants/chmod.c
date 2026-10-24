/*
 * chmod.c - conforming variants of chmod(), fchmodat() and fchmod(), for
 * the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

#define SETID (S_ISUID | S_ISGID)

static int fail(int e)
{
	errno = e;
	return -1;
}

static int in_groups(gid_t g)
{
	gid_t gs[256];
	int n = getgroups(256, gs), i;

	if (g == getegid())
		return 1;
	for (i = 0; i < n; i++)
		if (gs[i] == g)
			return 1;
	return 0;
}

/*
 * The mode this system applies, given the file's status before the call
 * (st), or -1 with errno set when it refuses.
 *
 * "setid_ignored": an implementation-defined restriction makes S_ISUID
 * and S_ISGID in mode ignored (XSH chmod() DESCRIPTION allows this), so
 * both keep the values they had. On this system every new directory also
 * has S_ISGID set (see mkdir() below), so the kept bit shows.
 * "setgid_eperm": a process without appropriate privileges that asks for
 * S_ISGID on a regular file of a group it is not in is refused with EPERM
 * and the mode is unchanged, instead of having the bit cleared. EPERM is a
 * listed error and XSH 2.3 lets it be used in other circumstances; the
 * BSDs do this.
 * "setuid_eperm": likewise, a process without appropriate privileges is
 * refused with EPERM, mode unchanged, when it asks for S_ISUID.
 */
static long apply(const char *v, const struct stat *st, mode_t mode)
{
	if (IS("setid_ignored"))
		return (long)((mode & ~(mode_t)SETID) | (st->st_mode & SETID));
	if (IS("setgid_eperm") && (mode & S_ISGID) && geteuid() != 0 && S_ISREG(st->st_mode) &&
	    !in_groups(st->st_gid))
		return fail(EPERM);
	if (IS("setuid_eperm") && (mode & S_ISUID) && geteuid() != 0)
		return fail(EPERM);
	return (long)mode;
}

int fchmodat(int fd, const char *path, mode_t mode, int flag)
{
	const char *v = variant_of("chmod.");
	struct stat st;
	long m;
	REAL(fchmodat);
	REAL(fstatat);

	if (v == NULL)
		return real_fchmodat(fd, path, mode, flag);
	/*
	 * "nofollow_works": with AT_SYMLINK_NOFOLLOW the mode of a symbolic
	 * link is changed and the call succeeds, which XSH fchmodat() describes
	 * (EOPNOTSUPP is only for systems that cannot). What lstat() then
	 * reports for the link's mode is unspecified (XBD 4.16 and XSH lstat()),
	 * so nothing visible changes here.
	 */
	if (IS("nofollow_works") && (flag & AT_SYMLINK_NOFOLLOW) &&
	    real_fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) == 0 && S_ISLNK(st.st_mode))
		return 0;
	if ((IS("setid_ignored") || IS("setgid_eperm") || IS("setuid_eperm")) &&
	    real_fstatat(fd, path, &st, flag & AT_SYMLINK_NOFOLLOW) == 0) {
		if ((m = apply(v, &st, mode)) < 0)
			return -1;
		mode = (mode_t)m;
	}
	if (real_fchmodat(fd, path, mode, flag) == 0)
		return 0;
	/*
	 * Not declared by any test: the other reading of disputed assertions
	 * (Q-8: ENOENT also applies when a lookup cannot find the last
	 * component, so XSH 2.3 lets it be reported). For a manual run. A
	 * trailing slash on an existing file stays ENOTDIR (only it applies).
	 */
	if (IS("enoent_lookup") && (errno == ENAMETOOLONG ||
				    (errno == ENOTDIR && real_fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) != 0 &&
				     strchr(path, '/') != NULL && path[strlen(path) - 1] != '/')))
		errno = ENOENT;
	return -1;
}

int chmod(const char *path, mode_t mode)
{
	return fchmodat(AT_FDCWD, path, mode, 0);
}

int fchmod(int fd, mode_t mode)
{
	const char *v = variant_of("chmod.");
	struct stat st;
	long m;
	REAL(fchmod);
	REAL(fstat);

	if ((IS("setid_ignored") || IS("setgid_eperm") || IS("setuid_eperm")) && real_fstat(fd, &st) == 0) {
		if ((m = apply(v, &st, mode)) < 0)
			return -1;
		mode = (mode_t)m;
	}
	return real_fchmod(fd, mode);
}

/* "setid_ignored": new directories carry S_ISGID (as for mkdir.sgid_bit, XSH mkdir() DESCRIPTION). */
static int make(int fd, const char *path, mode_t mode)
{
	const char *v = variant_of("chmod.");
	struct stat st;
	int r, saved;
	REAL(mkdirat);
	REAL(fchmodat);
	REAL(fstatat);

	r = real_mkdirat(fd, path, mode);
	if (r == 0 && IS("setid_ignored")) {
		saved = errno;
		if (real_fstatat(fd, path, &st, 0) == 0)
			(void)real_fchmodat(fd, path, (st.st_mode & 07777) | S_ISGID, 0);
		errno = saved;
	}
	return r;
}

int mkdir(const char *path, mode_t mode)
{
	return make(AT_FDCWD, path, mode);
}

int mkdirat(int fd, const char *path, mode_t mode)
{
	return make(fd, path, mode);
}

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; a time set to V is recorded as the
 * greatest supported value not above V, so stat() reports whole seconds.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("chmod.");

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
