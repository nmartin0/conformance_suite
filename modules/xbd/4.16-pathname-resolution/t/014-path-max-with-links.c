/*
 * When following a symbolic link makes the combined pathname longer than
 * {PATH_MAX}, the system may fail the resolution; if it does it reports
 * ENAMETOOLONG, and if it does not the file is the one the short name
 * leads to.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.014
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
	long pmax;
	char *content, *path;
	size_t clen, i;
	struct stat a, b;
	int r, e;

	pc_begin("XBD.4.16-pathname-resolution.014");
	pmax = pathconf(".", _PC_PATH_MAX);
	if (pmax < 0)
		pc_notinuse("{PATH_MAX} has no limit here");
	if (pmax > 8192)
		pc_notinuse("{PATH_MAX} is %ld here: too large to try", pmax);
	pc_make_dir("t");
	pc_make_dir("t/leaf");
	write_str("t/leaf/f", "x");
	content = malloc((size_t)pmax + 64);
	path = malloc((size_t)pmax + 64);
	if (content == NULL || path == NULL)
		pc_unresolved("malloc failed");
	/* contents that name t/leaf through a long run of "./" components, close to {PATH_MAX} */
	clen = 0;
	for (i = 0; clen + 2 < (size_t)pmax - 40; i++) {
		content[clen++] = '.';
		content[clen++] = '/';
	}
	(void)strcpy(content + clen, "t/leaf");
	if (symlink(content, "big") != 0) {
		e = errno;
		pc_notinuse("a symbolic link whose contents are %lu bytes cannot be made here: %s", (unsigned long)strlen(content), pc_errname(e));
	}
	/* the rest of the pathname is short, but joined to the contents it is longer than {PATH_MAX} */
	(void)strcpy(path, "big/");
	for (i = 0; i < 24; i++)
		(void)strcat(path, "./");
	(void)strcat(path, "f");
	pc_test();
	a = S("t/leaf/f");
	r = stat(path, &b);
	e = errno;
	if (r == 0)
		pc_check(same(&a, &b), "the resolved file is not t/leaf/f");
	else
		pc_check(e == ENAMETOOLONG, "resolving a pathname that grows past {PATH_MAX} through a link failed with %s, expected success or ENAMETOOLONG", pc_errname(e));
	pc_pass();
}
