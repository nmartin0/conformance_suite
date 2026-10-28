/*
 * Symbolic links: followed in the middle of a pathname and at its end
 * (stat, open); acted on themselves at the end of a pathname with no
 * trailing slash by the functions that act on the link (lstat, readlink,
 * unlink, rename, symlink, and remove).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.006
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
	struct stat a, b;
	char buf[32];
	ssize_t n;
	int fd;

	pc_begin("XBD.4.16-pathname-resolution.006");
	pc_make_dir("d");
	write_str("d/f", "data");
	write_str("tgt", "target");
	pc_make_symlink("d", "sl_d");
	pc_make_symlink("tgt", "sl_t");
	pc_make_symlink("sl_t", "sl_t2");
	pc_test();
	/* followed in the middle */
	a = S("d/f");
	b = S("sl_d/f");
	pc_check(same(&a, &b), "sl_d/f (link to a directory, then a name) is not d/f");
	/* followed at the end by stat and open */
	a = S("tgt");
	b = S("sl_t");
	pc_check(same(&a, &b), "stat of a link to a file did not describe the file");
	b = S("sl_t2");
	pc_check(same(&a, &b), "stat of a link to a link to a file did not describe the file");
	fd = open("sl_t", O_RDONLY);
	pc_check(fd >= 0, "open of a link failed: %s", pc_errname(errno));
	n = read(fd, buf, sizeof buf);
	(void)close(fd);
	pc_check(n == 6 && memcmp(buf, "target", 6) == 0, "open of a link did not open the file it leads to");
	/* the functions that act on the link itself */
	b = L("sl_t");
	pc_check(S_ISLNK(b.st_mode) && !same(&a, &b), "lstat of a link did not describe the link");
	n = readlink("sl_t", buf, sizeof buf);
	pc_check(n == 3 && memcmp(buf, "tgt", 3) == 0, "readlink did not return the link's contents");
	pc_check(symlink("x", "sl_t") == -1 && errno == EEXIST, "symlink onto an existing link did not fail with EEXIST (%s)", pc_errname(errno));
	pc_check(rename("sl_t2", "sl_moved") == 0, "rename of a link failed: %s", pc_errname(errno));
	pc_check(!exists("sl_t2") && S_ISLNK(L("sl_moved").st_mode), "rename did not move the link itself");
	pc_check(exists("sl_t"), "sl_t disappeared when sl_t2 was renamed");
	b = S("tgt");
	pc_check(same(&a, &b), "tgt changed when its link was renamed");
	pc_check(unlink("sl_t") == 0, "unlink of a link failed: %s", pc_errname(errno));
	pc_check(!exists("sl_t") && exists("tgt"), "unlink removed the target and not the link");
	pc_check(remove("sl_moved") == 0 && exists("tgt") && !exists("sl_moved"), "remove of a link did not remove just the link");
	/* a link in the middle is followed by those same functions */
	pc_check(unlink("sl_d/f") == 0 && !exists("d/f"), "unlink(\"sl_d/f\") did not remove d/f");
	pc_pass();
}
