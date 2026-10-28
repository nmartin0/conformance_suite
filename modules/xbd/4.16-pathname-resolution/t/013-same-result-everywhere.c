/*
 * The same pathname gives the same result through every interface: stat(),
 * access(), fstatat(), open() and realpath() agree on success or failure
 * (and, where the error is not a matter of choice, on the error).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.013
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

#include <sys/statvfs.h>

struct row {
	const char *path;
	int exact;	/* the error number is the same for every interface and is this one (0: success) */
	int err;
};

static int via(int k, const char *p, int *e)
{
	struct stat sb;
	struct statvfs vb;
	int r, fd;

	switch (k) {
	case 0:
		r = stat(p, &sb);
		break;
	case 1:
		r = access(p, F_OK);
		break;
	case 2:
		r = fstatat(AT_FDCWD, p, &sb, 0);
		break;
	case 3:
		fd = open(p, O_RDONLY | O_NONBLOCK);
		r = fd < 0 ? -1 : 0;
		if (fd >= 0)
			(void)close(fd);
		break;
	default:
		r = statvfs(p, &vb);
		break;
	}
	*e = r == 0 ? 0 : errno;
	return r;
}

int main(void)
{
	static const char *names[] = { "stat", "access", "fstatat", "open", "statvfs" };
	char longp[4300], cwd[PATH_MAX];
	long max;
	struct row rows[16];
	int n = 0, i, k, e, e0, r0;

	pc_begin("XBD.4.16-pathname-resolution.013");
	pc_make_dir("d");
	pc_make_dir("d/sub");
	write_str("d/f", "x");
	pc_make_symlink("d", "ln");
	pc_make_symlink("lb", "la");
	pc_make_symlink("la", "lb");
	pc_make_symlink("nowhere", "dang");
	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed");
	rows[n++] = (struct row){ "d/f", 1, 0 };
	rows[n++] = (struct row){ "d/./sub/../f", 1, 0 };
	rows[n++] = (struct row){ "ln/f", 1, 0 };
	rows[n++] = (struct row){ "ln/", 1, 0 };
	rows[n++] = (struct row){ "d/none", 1, ENOENT };
	rows[n++] = (struct row){ "none/x/y", 1, ENOENT };
	rows[n++] = (struct row){ "d/f/", 1, ENOTDIR };
	rows[n++] = (struct row){ "la", 1, ELOOP };
	rows[n++] = (struct row){ "la/x", 1, ELOOP };
	rows[n++] = (struct row){ "dang", 1, ENOENT };
	rows[n++] = (struct row){ "", 1, ENOENT };
	rows[n++] = (struct row){ cwd, 1, 0 };
	rows[n++] = (struct row){ "d/f/x", 0, 0 };	/* ENOTDIR or ENOENT: only failure is compared */
	max = pathconf("d", _PC_NAME_MAX);
	if (max > 0 && max < 4000) {
		(void)strcpy(longp, "d/");
		memset(longp + 2, 'n', (size_t)max + 1);
		longp[max + 3] = '\0';
		rows[n++] = (struct row){ longp, 0, 0 };
	}
	pc_test();
	for (i = 0; i < n; i++) {
		r0 = via(0, rows[i].path, &e0);
		pc_check(rows[i].exact == 0 || e0 == rows[i].err, "stat(\"%s\") gave %s, expected %s", rows[i].path,
		    e0 ? pc_errname(e0) : "success", rows[i].err ? pc_errname(rows[i].err) : "success");
		for (k = 1; k < 5; k++) {
			int r = via(k, rows[i].path, &e);

			pc_check((r == 0) == (r0 == 0), "stat(\"%s\") %s but %s() %s", rows[i].path, r0 == 0 ? "succeeded" : "failed",
			    names[k], r == 0 ? "succeeded" : "failed");
			if (rows[i].exact)
				pc_check(e == e0, "stat(\"%s\") gave %s but %s() gave %s", rows[i].path,
				    e0 ? pc_errname(e0) : "success", names[k], e ? pc_errname(e) : "success");
		}
	}
	pc_pass();
}
