/*
 * Dot is the directory itself, dot-dot its parent; dot-dot after a
 * symbolic link is the parent of the directory the link leads to; neither
 * can follow a non-directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.010
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
	struct stat a, b, c;

	pc_begin("XBD.4.16-pathname-resolution.010");
	pc_make_dir("d");
	pc_make_dir("d/sub");
	write_str("d/sub/x", "sub");
	write_str("d/y", "y");
	pc_make_symlink("d/sub", "ln");
	pc_test();
	a = S("d");
	b = S("d/.");
	pc_check(same(&a, &b), "d/. is not d");
	b = S("d/sub/..");
	pc_check(same(&a, &b), "d/sub/.. is not d");
	b = S("./d/./.");
	pc_check(same(&a, &b), "./d/./. is not d");
	b = S("d/sub/../sub/../.");
	pc_check(same(&a, &b), "d/sub/../sub/../. is not d");
	a = S(".");
	b = S("d/sub/../..");
	pc_check(same(&a, &b), "d/sub/../.. is not the working directory");
	b = S("./.");
	pc_check(same(&a, &b), "./. is not the working directory");
	a = S("d/sub/x");
	b = S("d/./sub/../sub/./x");
	pc_check(same(&a, &b), "d/./sub/../sub/./x is not d/sub/x");
	/* dot-dot after a link: the parent of the target */
	a = S("d");
	b = S("ln/..");
	c = S(".");
	pc_check(same(&a, &b), "ln/.. (ln a link to d/sub) is not the parent of d/sub");
	pc_check(!same(&c, &b), "ln/.. was taken as the directory that contains the link");
	a = S("d/y");
	b = S("ln/../y");
	pc_check(same(&a, &b), "ln/../y is not d/y");
	/* the working directory's dot-dot */
	pc_check(chdir("d/sub") == 0, "chdir(d/sub) failed");
	a = S("..");
	b = S("../sub/..");
	pc_check(same(&a, &b), "../sub/.. is not ..");
	b = S("../y");
	c = S("../../d/y");
	pc_check(same(&b, &c), "../y and ../../d/y differ");
	pc_pass();
}
