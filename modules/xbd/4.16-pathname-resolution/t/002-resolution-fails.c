/*
 * A pathname fails to resolve when a component cannot be found in the
 * directory before it: a missing name, or a prefix that is not a directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.002
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int same(const struct stat *a, const struct stat *b)
{
	return a->st_dev == b->st_dev && a->st_ino == b->st_ino;
}

static struct stat S(const char *p)
{
	struct stat sb;

	pc_check(stat(p, &sb) == 0, "stat(\"%s\") failed: %s", p, pc_errname(errno));
	return sb;
}

static struct stat L(const char *p)
{
	struct stat sb;

	pc_check(lstat(p, &sb) == 0, "lstat(\"%s\") failed: %s", p, pc_errname(errno));
	return sb;
}

/* stat(p) must fail with e1 (or e2 when non-zero). */
static void stat_fails(const char *p, int e1, int e2)
{
	struct stat sb;
	int r = stat(p, &sb), e = errno;

	pc_check(r == -1, "stat(\"%s\") succeeded, but the pathname cannot be resolved", p);
	pc_check(e == e1 || (e2 != 0 && e == e2), "stat(\"%s\") failed with %s, expected %s%s%s", p, pc_errname(e),
	    pc_errname(e1), e2 ? " or " : "", e2 ? pc_errname(e2) : "");
}

static int exists(const char *p)
{
	struct stat sb;

	return lstat(p, &sb) == 0;
}

static void write_str(const char *p, const char *s)
{
	pc_make_file(p, s);
}

static char *slurp(const char *p, char *buf, size_t n)
{
	int fd = open(p, O_RDONLY);
	ssize_t r;

	pc_check(fd >= 0, "open(\"%s\") failed: %s", p, pc_errname(errno));
	r = read(fd, buf, n - 1);
	(void)close(fd);
	pc_check(r >= 0, "read of \"%s\" failed", p);
	buf[r] = '\0';
	return buf;
}

int main(void)
{
	struct stat sb;
	DIR *dp;

	pc_begin("XBD.4.16-pathname-resolution.002");
	pc_make_dir("d");
	write_str("d/f", "x");
	pc_test();
	stat_fails("d/none", ENOENT, 0);
	stat_fails("d/none/x", ENOENT, 0);
	stat_fails("none/x/y", ENOENT, 0);
	stat_fails("d/f/x", ENOTDIR, ENOENT);
	stat_fails("d/f/.", ENOTDIR, ENOENT);
	stat_fails("d/f/..", ENOTDIR, ENOENT);
	pc_check(open("d/none/x", O_RDONLY) == -1 && errno == ENOENT, "open of d/none/x did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(access("d/none/x", F_OK) == -1 && errno == ENOENT, "access of d/none/x did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(chdir("d/none") == -1 && errno == ENOENT, "chdir to d/none did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(unlink("d/none/x") == -1 && errno == ENOENT, "unlink of d/none/x did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(chdir("d/f") == -1 && errno == ENOTDIR, "chdir to a regular file did not fail with ENOTDIR (%s)", pc_errname(errno));
	dp = opendir("d/f");
	pc_check(dp == NULL && errno == ENOTDIR, "opendir of a regular file did not fail with ENOTDIR (%s)", pc_errname(errno));
	pc_check(lstat("d/f/x", &sb) == -1, "lstat of d/f/x succeeded");
	pc_check(stat("d/f", &sb) == 0, "stat(d/f) failed: %s", pc_errname(errno));
	pc_pass();
}
