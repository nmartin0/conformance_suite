/*
 * Absolute and relative pathnames: an absolute one starts at the root
 * whatever the working directory, a relative one at the working directory (and
 * follows chdir()), and for the *at() interfaces at the directory the
 * descriptor names, not the working directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.001
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
	char cwd[PATH_MAX], absf[PATH_MAX + 16], buf[16];
	struct stat a, e, b, r1, r2;
	int dfd, fd;

	pc_begin("XBD.4.16-pathname-resolution.001");
	pc_make_dir("d");
	pc_make_dir("e");
	write_str("d/f", "in d");
	write_str("e/f", "in e");
	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed: %s", pc_errname(errno));
	if (strlen(cwd) + 8 >= sizeof absf)
		pc_unresolved("the working directory name is too long");
	(void)snprintf(absf, sizeof absf, "%s/d/f", cwd);
	dfd = open("d", O_RDONLY);
	if (dfd < 0)
		pc_unresolved("open(d) failed: %s", pc_errname(errno));
	pc_test();
	a = S("d/f");
	e = S("e/f");
	pc_check(!same(&a, &e), "d/f and e/f are the same file");
	b = S(absf);
	pc_check(same(&a, &b), "the absolute pathname of d/f does not resolve to d/f");
	r1 = S("/");
	pc_check(S_ISDIR(r1.st_mode), "\"/\" is not a directory");
	r2 = S("/.");
	pc_check(same(&r1, &r2), "\"/.\" is not the root directory");
	pc_check(chdir("e") == 0, "chdir(e) failed: %s", pc_errname(errno));
	b = S("f");
	pc_check(same(&b, &e), "after chdir(\"e\") the relative name f does not resolve to e/f");
	b = S(absf);
	pc_check(same(&b, &a), "after chdir(\"e\") the absolute pathname no longer resolves to d/f");
	pc_check(fstatat(dfd, "f", &b, 0) == 0, "fstatat(dirfd, \"f\") failed: %s", pc_errname(errno));
	pc_check(same(&b, &a), "fstatat(dirfd of d, \"f\") with the working directory e resolved f in the working directory");
	fd = openat(dfd, "f", O_RDONLY);
	pc_check(fd >= 0, "openat(dirfd, \"f\") failed: %s", pc_errname(errno));
	pc_check(read(fd, buf, 4) == 4 && memcmp(buf, "in d", 4) == 0, "openat(dirfd of d, \"f\") did not open d/f");
	(void)close(fd);
	pc_check(fstatat(dfd, absf, &b, 0) == 0 && same(&b, &a), "an absolute pathname given to fstatat() is not resolved from the root");
	pc_check(fstatat(AT_FDCWD, "f", &b, 0) == 0 && same(&b, &e), "fstatat(AT_FDCWD, \"f\") is not the working directory's f");
	pc_check(chdir("/") == 0, "chdir(\"/\") failed");
	b = S(".");
	pc_check(same(&b, &r1), "after chdir(\"/\") the name \".\" is not the root directory");
	pc_check(chdir(cwd) == 0, "chdir back failed");
	pc_pass();
}
