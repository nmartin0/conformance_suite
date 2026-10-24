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
