/*
 * fchmod.c - conforming variants for the fchmod() tests (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <sys/stat.h>
#include "pcvariant.h"

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; a time set to V is recorded as the
 * greatest supported value not above V, so stat() reports whole seconds.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("fchmod.");

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
