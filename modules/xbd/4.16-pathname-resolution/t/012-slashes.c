/*
 * Repeated slashes count as one, and three or more leading slashes are
 * one root; the same name reached with extra slashes is the same entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.012
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
	char cwd[PATH_MAX], p[PATH_MAX + 32];
	struct stat a, b;

	pc_begin("XBD.4.16-pathname-resolution.012");
	pc_make_dir("d");
	pc_make_dir("d/e");
	write_str("d/e/f", "x");
	if (getcwd(cwd, sizeof cwd) == NULL || cwd[0] != '/' || strlen(cwd) + 16 >= sizeof p)
		pc_unresolved("getcwd failed or gave an unusable name");
	pc_test();
	a = S("d/e/f");
	b = S("d//e///f");
	pc_check(same(&a, &b), "d//e///f is not d/e/f");
	b = S("./d//./e//f");
	pc_check(same(&a, &b), "./d//./e//f is not d/e/f");
	b = S("d/e/f");
	(void)snprintf(p, sizeof p, "%s/d/e/f", cwd);
	b = S(p);
	pc_check(same(&a, &b), "the absolute pathname is not d/e/f");
	(void)snprintf(p, sizeof p, "//%s/d/e/f", cwd);	/* three or more leading slashes */
	b = S(p);
	pc_check(same(&a, &b), "three leading slashes are not taken as one");
	(void)snprintf(p, sizeof p, "////%s/d/e/f", cwd);
	b = S(p);
	pc_check(same(&a, &b), "five leading slashes are not taken as one");
	(void)snprintf(p, sizeof p, "///%s//d///e//f", cwd);
	b = S(p);
	pc_check(same(&a, &b), "leading and inner repeated slashes are not taken as one");
	a = S("/");
	b = S("////");
	pc_check(same(&a, &b), "\"////\" is not the root directory");
	b = S("/./");
	pc_check(same(&a, &b), "\"/./\" is not the root directory");
	pc_pass();
}
