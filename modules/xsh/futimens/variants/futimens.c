/*
 * futimens.c - conforming variants of futimens() and utimensat(), for the
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
#include <sys/time.h>
#include <time.h>
#include "pcvariant.h"

/*
 * "utime_now_m1": UTIME_NOW is -1 and UTIME_OMIT is -2 (the header half is
 * futimens.utime_now_m1.h). The values the test now uses mean "now" and
 * "omit"; this system's own values are then ordinary out-of-range tv_nsec
 * values, which XSH futimens() requires to fail with EINVAL.
 */
static int translate(const struct timespec *in, struct timespec *out)
{
	int i;

	for (i = 0; i < 2; i++) {
		out[i] = in[i];
		if (in[i].tv_nsec == UTIME_NOW || in[i].tv_nsec == UTIME_OMIT) {
			errno = EINVAL;
			return -1;
		}
		if (in[i].tv_nsec == -1L)
			out[i].tv_nsec = UTIME_NOW;
		else if (in[i].tv_nsec == -2L)
			out[i].tv_nsec = UTIME_OMIT;
	}
	return 0;
}

int futimens(int fd, const struct timespec times[2])
{
	const char *v = variant_of("futimens.");
	struct timespec t[2];
	REAL(futimens);

	if (IS("utime_now_m1") && times != NULL) {
		if (translate(times, t) != 0)
			return -1;
		return real_futimens(fd, t);
	}
	return real_futimens(fd, times);
}

int utimensat(int dirfd, const char *path, const struct timespec times[2], int flag)
{
	const char *v = variant_of("futimens.");
	struct timespec t[2];
	REAL(utimensat);

	if (IS("utime_now_m1") && times != NULL) {
		if (translate(times, t) != 0)
			return -1;
		return real_utimensat(dirfd, path, t, flag);
	}
	return real_utimensat(dirfd, path, times, flag);
}

/*
 * "res_1s": the file system keeps timestamps to the whole second. XBD 4.12
 * leaves the resolution to the implementation as long as it is no coarser
 * than one second, and a time to be stored becomes the greatest value the
 * file system supports that is not greater than it. Truncating every
 * timestamp the stat() family reports to the second is what such a file
 * system shows: the stored time (from futimens(), utimensat(), utimes(),
 * or the current time for marked updates) rounded down.
 */
static void whole_seconds(struct stat *st)
{
	st->st_atim.tv_nsec = 0;
	st->st_mtim.tv_nsec = 0;
	st->st_ctim.tv_nsec = 0;
}

static int res_1s(void)
{
	const char *v = variant_of("futimens.");

	return IS("res_1s");
}

int stat(const char *restrict path, struct stat *restrict st)
{
	int r;
	REAL(stat);

	r = real_stat(path, st);
	if (r == 0 && res_1s())
		whole_seconds(st);
	return r;
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	int r;
	REAL(lstat);

	r = real_lstat(path, st);
	if (r == 0 && res_1s())
		whole_seconds(st);
	return r;
}

int fstat(int fd, struct stat *st)
{
	int r;
	REAL(fstat);

	r = real_fstat(fd, st);
	if (r == 0 && res_1s())
		whole_seconds(st);
	return r;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	int r;
	REAL(fstatat);

	r = real_fstatat(fd, path, st, flag);
	if (r == 0 && res_1s())
		whole_seconds(st);
	return r;
}

/*
 * "utimes_round": utimes() rounds each time to the nearest second. XSH
 * utimes() (futimens.html DESCRIPTION) is utimensat() with microseconds,
 * except that it says rounding towards the nearest second may occur. Only
 * in-range tv_usec values are rounded; others go through unchanged, so the
 * system's own range check still applies to them.
 */
int utimes(const char *path, const struct timeval times[2])
{
	const char *v = variant_of("futimens.");
	struct timeval t[2];
	int i;
	REAL(utimes);

	if (IS("utimes_round") && times != NULL) {
		for (i = 0; i < 2; i++) {
			t[i] = times[i];
			if (t[i].tv_usec >= 500000 && t[i].tv_usec < 1000000) {
				t[i].tv_sec++;
				t[i].tv_usec = 0;
			} else if (t[i].tv_usec >= 0 && t[i].tv_usec < 500000) {
				t[i].tv_usec = 0;
			}
		}
		return real_utimes(path, t);
	}
	return real_utimes(path, times);
}
