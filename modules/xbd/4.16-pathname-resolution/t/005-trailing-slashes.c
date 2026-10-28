/*
 * Trailing slashes: a pathname with a non-slash character and trailing
 * slashes resolves only if its last component is (or, through symbolic
 * links, leads to) a directory, or is a directory still to be created; and
 * a pathname of slashes alone is the root.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.005
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
	struct stat sb, r;
	int fd;

	pc_begin("XBD.4.16-pathname-resolution.005");
	write_str("f", "x");
	pc_make_dir("d");
	pc_make_symlink("f", "sf");
	pc_make_symlink("d", "sd");
	pc_make_symlink("none", "dang");
	pc_test();
	/* existing directory, and links to one */
	pc_check(stat("d/", &sb) == 0 && S_ISDIR(sb.st_mode), "stat(\"d/\") did not give the directory");
	pc_check(stat("d///", &sb) == 0 && S_ISDIR(sb.st_mode), "stat(\"d///\") did not give the directory");
	pc_check(stat("sd/", &sb) == 0 && S_ISDIR(sb.st_mode), "stat(\"sd/\") (link to a directory) did not give the directory");
	pc_check(lstat("sd/", &sb) == 0 && S_ISDIR(sb.st_mode), "lstat(\"sd/\") described the link, not the directory it leads to");
	pc_check(lstat("sd", &sb) == 0 && S_ISLNK(sb.st_mode), "lstat(\"sd\") did not describe the link");
	fd = open("d/", O_RDONLY);
	pc_check(fd >= 0, "open(\"d/\") failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_check(access("d/", F_OK) == 0, "access(\"d/\") failed: %s", pc_errname(errno));
	/* not a directory */
	stat_fails("f/", ENOTDIR, 0);
	stat_fails("f//", ENOTDIR, 0);
	stat_fails("sf/", ENOTDIR, 0);
	pc_check(lstat("f/", &sb) == -1 && errno == ENOTDIR, "lstat(\"f/\") did not fail with ENOTDIR (%s)", pc_errname(errno));
	pc_check(lstat("sf/", &sb) == -1 && errno == ENOTDIR, "lstat(\"sf/\") (link to a file) did not fail with ENOTDIR (%s)", pc_errname(errno));
	pc_check(open("f/", O_RDONLY) == -1 && errno == ENOTDIR, "open(\"f/\") did not fail with ENOTDIR (%s)", pc_errname(errno));
	pc_check(access("f/", F_OK) == -1 && errno == ENOTDIR, "access(\"f/\") did not fail with ENOTDIR (%s)", pc_errname(errno));
	pc_check(chdir("f/") == -1 && errno == ENOTDIR, "chdir(\"f/\") did not fail with ENOTDIR (%s)", pc_errname(errno));
	stat_fails("dang/", ENOENT, 0);
	pc_check(exists("f"), "f disappeared");
	/* to be created, as a directory */
	pc_check(mkdir("nd/", 0700) == 0, "mkdir(\"nd/\") failed: %s", pc_errname(errno));
	pc_check(stat("nd", &sb) == 0 && S_ISDIR(sb.st_mode), "mkdir(\"nd/\") did not make a directory");
	pc_check(mkdir("nd2//", 0700) == 0 && stat("nd2", &sb) == 0 && S_ISDIR(sb.st_mode), "mkdir(\"nd2//\") did not make a directory");
	/* to be created, but not as a directory: must not make a non-directory */
	fd = open("newfile/", O_WRONLY | O_CREAT, 0600);
	pc_check(fd == -1, "open(\"newfile/\", O_CREAT) succeeded; a pathname with a trailing slash can only be a directory");
	pc_check(!exists("newfile"), "open(\"newfile/\", O_CREAT) left an entry behind");
	/* only slashes: the root */
	r = S("/");
	pc_check(stat("///", &sb) == 0 && same(&sb, &r), "stat(\"///\") is not the root directory");
	pc_pass();
}
