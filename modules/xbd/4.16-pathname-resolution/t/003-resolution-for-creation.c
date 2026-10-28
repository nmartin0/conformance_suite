/*
 * When an entry is to be created, only the path prefix is resolved: a
 * new name below an existing directory is created, one below a missing
 * directory is not (and nothing is made on the way), and an existing last
 * component is simply found.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.003
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
	int fd;

	pc_begin("XBD.4.16-pathname-resolution.003");
	pc_make_dir("d");
	write_str("d/old", "x");
	pc_test();
	fd = open("d/new", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "creating d/new failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_check(mkdir("d/sub", 0700) == 0, "mkdir d/sub failed: %s", pc_errname(errno));
	pc_check(symlink("t", "d/ln") == 0, "symlink d/ln failed: %s", pc_errname(errno));
	pc_check(mkfifo("d/ff", 0600) == 0, "mkfifo d/ff failed: %s", pc_errname(errno));
	pc_check(link("d/old", "d/hard") == 0, "link d/hard failed: %s", pc_errname(errno));
	pc_check(rename("d/hard", "d/hard2") == 0, "rename to d/hard2 failed: %s", pc_errname(errno));
	pc_check(exists("d/new") && exists("d/sub") && exists("d/ln") && exists("d/ff") && exists("d/hard2") && !exists("d/hard"), "the entries are not as expected after creating them");
	/* missing prefix */
	pc_check(open("d/m1/new", O_WRONLY | O_CREAT, 0600) == -1 && errno == ENOENT, "creating below a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(mkdir("d/m2/sub", 0700) == -1 && errno == ENOENT, "mkdir below a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(symlink("t", "d/m3/ln") == -1 && errno == ENOENT, "symlink below a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(link("d/old", "d/m4/l") == -1 && errno == ENOENT, "link below a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(rename("d/old", "d/m5/y") == -1 && errno == ENOENT, "rename into a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(mkfifo("d/m6/ff", 0600) == -1 && errno == ENOENT, "mkfifo below a missing directory did not fail with ENOENT (%s)", pc_errname(errno));
	pc_check(!exists("d/m1") && !exists("d/m2") && !exists("d/m3") && !exists("d/m4") && !exists("d/m5") && !exists("d/m6"), "a failed creation left a name behind");
	pc_check(exists("d/old"), "d/old disappeared after the failed rename");
	/* last component present */
	pc_check(open("d/old", O_WRONLY | O_CREAT | O_EXCL, 0600) == -1 && errno == EEXIST, "O_CREAT|O_EXCL of an existing name did not fail with EEXIST (%s)", pc_errname(errno));
	pc_check(mkdir("d/sub", 0700) == -1 && errno == EEXIST, "mkdir of an existing name did not fail with EEXIST (%s)", pc_errname(errno));
	pc_pass();
}
