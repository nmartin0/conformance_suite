/*
 * chown.c - conforming variants of chown() and fchownat(), and of the
 * timestamps the chown(), fchown(), truncate() and ftruncate() tests read,
 * for the variant checks (CI only).
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
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "coarse_times": the file system keeps timestamps to whole seconds.
 * XBD 4.12 (File Times Update) leaves the resolution of file timestamps
 * implementation-defined, bounded only at one second, and has a timestamp
 * set to the greatest supported value not after the time it is given; so
 * every status call reports the seconds only. A test that waits for a
 * timestamp to move must allow for at least a second.
 */
static void coarse(struct stat *st)
{
	st->st_atim.tv_nsec = 0;
	st->st_mtim.tv_nsec = 0;
	st->st_ctim.tv_nsec = 0;
}

static int coarse_on(void)
{
	const char *v = variant_of("chown.");

	return IS("coarse_times");
}

int stat(const char *restrict path, struct stat *restrict st)
{
	REAL(stat);
	int r = real_stat(path, st);

	if (r == 0 && coarse_on())
		coarse(st);
	return r;
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	REAL(lstat);
	int r = real_lstat(path, st);

	if (r == 0 && coarse_on())
		coarse(st);
	return r;
}

int fstat(int fd, struct stat *st)
{
	REAL(fstat);
	int r = real_fstat(fd, st);

	if (r == 0 && coarse_on())
		coarse(st);
	return r;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	REAL(fstatat);
	int r = real_fstatat(fd, path, st, flag);

	if (r == 0 && coarse_on())
		coarse(st);
	return r;
}

#if defined(__GLIBC__)
/* glibc may route the calls above to their 64-bit names. */
static void coarse64(struct stat64 *st)
{
	st->st_atim.tv_nsec = 0;
	st->st_mtim.tv_nsec = 0;
	st->st_ctim.tv_nsec = 0;
}

int stat64(const char *restrict path, struct stat64 *restrict st)
{
	REAL(stat64);
	int r = real_stat64(path, st);

	if (r == 0 && coarse_on())
		coarse64(st);
	return r;
}

int lstat64(const char *restrict path, struct stat64 *restrict st)
{
	REAL(lstat64);
	int r = real_lstat64(path, st);

	if (r == 0 && coarse_on())
		coarse64(st);
	return r;
}

int fstat64(int fd, struct stat64 *st)
{
	REAL(fstat64);
	int r = real_fstat64(fd, st);

	if (r == 0 && coarse_on())
		coarse64(st);
	return r;
}

int fstatat64(int fd, const char *restrict path, struct stat64 *restrict st, int flag)
{
	REAL(fstatat64);
	int r = real_fstatat64(fd, path, st, flag);

	if (r == 0 && coarse_on())
		coarse64(st);
	return r;
}
#endif

/*
 * "at_alt": AT_FDCWD is -2 and AT_SYMLINK_NOFOLLOW is 0x4000 (the header
 * half is chown.at_alt.h). XBD <fcntl.h> names these constants without
 * fixing their values: AT_FDCWD need only be a value that is no file
 * descriptor, and AT_SYMLINK_NOFOLLOW a flag bit. This system's own values
 * are then ordinary: its AT_FDCWD is a descriptor that is not open (EBADF
 * for a relative path; ignored for an absolute one), and its
 * AT_SYMLINK_NOFOLLOW an unknown flag bit, which fchownat() may reject
 * with EINVAL (chown.html, "may fail").
 */
#define ALT_FDCWD (-2)
#define ALT_NOFOLLOW 0x4000

int fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag)
{
	const char *v = variant_of("chown.");
	int f = 0;
	REAL(fchownat);

	if (IS("at_alt")) {
		if (flag & ~ALT_NOFOLLOW) {
			errno = EINVAL;
			return -1;
		}
		if (flag & ALT_NOFOLLOW)
			f = AT_SYMLINK_NOFOLLOW;
		if (fd == ALT_FDCWD)
			fd = AT_FDCWD;
		else if (fd == AT_FDCWD && path[0] != '/') {
			errno = EBADF;
			return -1;
		}
		return real_fchownat(fd, path, owner, group, f);
	}
	return real_fchownat(fd, path, owner, group, flag);
}
