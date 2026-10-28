/*
 * A symbolic link whose contents are the empty string either fails to
 * resolve with ENOENT, or stands for the directory that contains it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.008
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
	struct stat a, b;
	int r, e;

	pc_begin("XBD.4.16-pathname-resolution.008");
	write_str("f", "x");
	if (symlink("", "es") != 0) {
		e = errno;
		pc_notinuse("a symbolic link with empty contents cannot be made here: %s", pc_errname(e));
	}
	pc_test();
	pc_check(lstat("es", &b) == 0 && S_ISLNK(b.st_mode), "lstat of the empty link did not describe a link");
	a = S(".");
	r = stat("es", &b);
	e = errno;
	if (r == 0)
		pc_check(same(&a, &b), "stat(\"es\") succeeded but is not the directory that contains the link");
	else
		pc_check(e == ENOENT, "stat(\"es\") failed with %s, expected ENOENT", pc_errname(e));
	a = S("f");
	r = stat("es/f", &b);
	e = errno;
	if (r == 0)
		pc_check(same(&a, &b), "stat(\"es/f\") succeeded but is not the file f of the directory that contains the link");
	else
		pc_check(e == ENOENT, "stat(\"es/f\") failed with %s, expected ENOENT", pc_errname(e));
	pc_pass();
}
