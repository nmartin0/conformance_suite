/*
 * A loop of symbolic links fails with ELOOP (links met as the last
 * component of a function that acts on the link are not followed); a chain
 * as long as {SYMLOOP_MAX} (at least 8) is followed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.009
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
	long n = 0;
	char a[32], b[32], p[64];
	int i;
	struct stat sb;

	pc_begin("XBD.4.16-pathname-resolution.009");
#ifdef SYMLOOP_MAX
	n = SYMLOOP_MAX;
#else
	n = sysconf(_SC_SYMLOOP_MAX);
#endif
	if (n < 8)
		n = 8;			/* the least value {SYMLOOP_MAX} can have */
	if (n > 200)
		n = 200;
	pc_make_dir("d");
	write_str("d/f", "x");
	pc_make_symlink("b", "a");
	pc_make_symlink("a", "b");
	pc_make_symlink("self", "self");
	pc_make_symlink("d", "c1");
	for (i = 2; i <= n; i++) {
		(void)snprintf(a, sizeof a, "c%d", i - 1);
		(void)snprintf(b, sizeof b, "c%d", i);
		pc_make_symlink(a, b);
	}
	pc_test();
	stat_fails("a", ELOOP, 0);
	stat_fails("a/x", ELOOP, 0);
	stat_fails("self", ELOOP, 0);
	stat_fails("self/x", ELOOP, 0);
	pc_check(open("a", O_RDONLY) == -1 && errno == ELOOP, "open of a link loop did not fail with ELOOP (%s)", pc_errname(errno));
	pc_check(access("b", F_OK) == -1 && errno == ELOOP, "access of a link loop did not fail with ELOOP (%s)", pc_errname(errno));
	pc_check(lstat("a", &sb) == 0 && S_ISLNK(sb.st_mode), "lstat of a link in a loop failed: %s", pc_errname(errno));
	pc_check(lstat("self", &sb) == 0 && S_ISLNK(sb.st_mode), "lstat of a link to itself failed: %s", pc_errname(errno));
	(void)snprintf(p, sizeof p, "c%ld/f", n);
	pc_check(stat(p, &sb) == 0, "a chain of %ld symbolic links (the smallest allowed limit) was not followed: stat(\"%s\") failed with %s", n, p, pc_errname(errno));
	pc_check(S_ISREG(sb.st_mode), "\"%s\" is not the regular file d/f", p);
	(void)snprintf(p, sizeof p, "c%ld", n);
	pc_check(stat(p, &sb) == 0 && S_ISDIR(sb.st_mode), "stat of the last link of the chain of %ld failed or did not give the directory (%s)", n, pc_errname(errno));
	pc_pass();
}
