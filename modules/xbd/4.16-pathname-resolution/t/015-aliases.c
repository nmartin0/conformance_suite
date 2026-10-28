/*
 * Several pathnames name one directory entry, and several directory
 * entries one file: all the spellings of a directory give the same
 * directory, all the spellings of a file the same file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.015
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
	char cwd[PATH_MAX], p[PATH_MAX + 32], base[PATH_MAX];
	struct stat a, b;
	char *slash;
	int i;
	const char *dirs[] = { "./d", "d/.", "d/sub/..", "d/", "ln", "ln/.", "./d/./sub/.." };
	const char *files[] = { "d/f", "./d/f", "d/./f", "d/sub/../f", "ln/f", "d/f2", "ln2" };

	pc_begin("XBD.4.16-pathname-resolution.015");
	pc_make_dir("d");
	pc_make_dir("d/sub");
	write_str("d/f", "x");
	if (link("d/f", "d/f2") != 0)
		pc_unresolved("link failed: %s", pc_errname(errno));
	pc_make_symlink("d", "ln");
	pc_make_symlink("d/f", "ln2");
	if (getcwd(cwd, sizeof cwd) == NULL || strlen(cwd) + 16 >= sizeof p)
		pc_unresolved("getcwd failed or gave an unusable name");
	pc_test();
	a = S("d");
	for (i = 0; i < 7; i++) {
		b = S(dirs[i]);
		pc_check(same(&a, &b), "\"%s\" is not the directory d", dirs[i]);
	}
	(void)snprintf(p, sizeof p, "%s/d", cwd);
	b = S(p);
	pc_check(same(&a, &b), "the absolute pathname of d is not d");
	(void)strcpy(base, cwd);
	slash = strrchr(base, '/');
	if (slash != NULL && slash[1] != '\0') {
		(void)snprintf(p, sizeof p, "../%s/d", slash + 1);
		b = S(p);
		pc_check(same(&a, &b), "\"%s\" is not the directory d", p);
	}
	a = S("d/f");
	for (i = 0; i < 7; i++) {
		b = S(files[i]);
		pc_check(same(&a, &b), "\"%s\" is not the file d/f", files[i]);
	}
	(void)snprintf(p, sizeof p, "%s/d/sub/../f", cwd);
	b = S(p);
	pc_check(same(&a, &b), "the absolute pathname d/sub/../f is not d/f");
	pc_pass();
}
