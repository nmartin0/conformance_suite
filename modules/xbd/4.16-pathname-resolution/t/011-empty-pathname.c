/*
 * The empty pathname is never resolved: every function given one as a
 * pathname fails with ENOENT, and nothing is created.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.011
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

static int enoent(long r)
{
	return r == -1 && errno == ENOENT;
}

int main(void)
{
	struct stat sb;
	char b[8];
	DIR *dp;
	int fd;

	pc_begin("XBD.4.16-pathname-resolution.011");
	write_str("f", "x");
	pc_test();
	pc_check(enoent(stat("", &sb)), "stat(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(lstat("", &sb)), "lstat(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(open("", O_RDONLY)), "open(\"\", O_RDONLY) did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(open("", O_WRONLY | O_CREAT, 0600)), "open(\"\", O_CREAT) did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(access("", F_OK)), "access(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(chdir("")), "chdir(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(unlink("")), "unlink(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(rmdir("")), "rmdir(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(mkdir("", 0700)), "mkdir(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(readlink("", b, sizeof b)), "readlink(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(fstatat(AT_FDCWD, "", &sb, 0)), "fstatat(AT_FDCWD, \"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(rename("", "g")), "rename(\"\", \"g\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(rename("f", "")), "rename(\"f\", \"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(link("f", "")), "link(\"f\", \"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(link("", "g")), "link(\"\", \"g\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(symlink("t", "")), "symlink(\"t\", \"\") did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(enoent(mkfifo("", 0600)), "mkfifo(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	dp = opendir("");
	pc_check(dp == NULL && errno == ENOENT, "opendir(\"\") did not fail with ENOENT (%s)", pc_errname(errno));
	fd = open(".", O_RDONLY);
	pc_check(fd >= 0, "open(\".\") failed");
	pc_check(enoent(fstatat(fd, "", &sb, 0)), "fstatat(dirfd, \"\") did not fail with ENOENT (%s)", pc_errname(errno));
	(void)close(fd);
	pc_check(exists("f") && !exists("g"), "a failed call changed the directory");
	pc_pass();
}
