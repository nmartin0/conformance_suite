/*
 * The contents of a symbolic link replace its name in the pathname: a
 * relative link is read from the directory containing the link (not the
 * working directory), an absolute one from the root, a chain of links is
 * followed, and a link of slashes alone leaves just those slashes in front of
 * the rest.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.007
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
	char cwd[PATH_MAX], absd[PATH_MAX + 8];
	struct stat a, b;

	pc_begin("XBD.4.16-pathname-resolution.007");
	pc_make_dir("sub");
	pc_make_dir("sub/deep");
	write_str("x", "cwd");
	write_str("sub/x", "sub");
	write_str("sub/deep/y", "deep");
	if (getcwd(cwd, sizeof cwd) == NULL || strlen(cwd) + 8 >= sizeof absd)
		pc_unresolved("getcwd failed or the working directory name is too long");
	(void)snprintf(absd, sizeof absd, "%s/sub", cwd);
	pc_make_symlink("x", "sub/rel");			/* sub/x */
	pc_make_symlink("../x", "sub/up");			/* ./x */
	pc_make_symlink("deep", "sub/dl");			/* sub/deep */
	pc_make_symlink(absd, "abs");				/* the absolute path of sub */
	pc_make_symlink("sub/rel", "chain1");			/* relative to the working directory */
	pc_make_symlink("chain1", "chain2");
	pc_make_symlink("/", "rootl");
	pc_make_symlink("///", "rootl3");
	pc_test();
	a = S("sub/x");
	b = S("sub/rel");
	pc_check(same(&a, &b), "a relative link was not read from the directory that contains the link");
	a = S("x");
	b = S("sub/up");
	pc_check(same(&a, &b), "a link containing \"..\" was not read from the directory that contains the link");
	a = S("sub/deep/y");
	b = S("sub/dl/y");
	pc_check(same(&a, &b), "sub/dl/y (relative link to a directory, then a name) is not sub/deep/y");
	a = S("sub/x");
	b = S("abs/x");
	pc_check(same(&a, &b), "a link to an absolute pathname was not read from the root");
	b = S("chain2");
	pc_check(same(&a, &b), "a chain of two links to sub/rel did not lead to sub/x");
	/* links of slashes alone */
	a = S("/");
	b = S("rootl");
	pc_check(same(&a, &b), "a link containing / is not the root directory");
	b = S("rootl3");
	pc_check(same(&a, &b), "a link containing /// is not the root directory");
	a = S(absd);
	{
		char viaroot[PATH_MAX + 16];

		(void)snprintf(viaroot, sizeof viaroot, "rootl%s", absd);
		b = S(viaroot);
		pc_check(same(&a, &b), "rootl followed by the absolute path of sub does not lead to sub");
		(void)snprintf(viaroot, sizeof viaroot, "rootl3%s", absd);
		b = S(viaroot);
		pc_check(same(&a, &b), "rootl3 followed by the absolute path of sub does not lead to sub");
		(void)snprintf(viaroot, sizeof viaroot, "rootl3/%s", absd);
		b = S(viaroot);
		pc_check(same(&a, &b), "rootl3 followed by extra slashes and the absolute path of sub does not lead to sub");
	}
	pc_pass();
}
