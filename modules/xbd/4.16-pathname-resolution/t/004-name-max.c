/*
 * A filename of {NAME_MAX} bytes can be used; a component one byte
 * longer is an error, at the end of a pathname and in the middle of it, and
 * nothing is created.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.16-pathname-resolution.004
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
	long max;
	char name[4200], p[4300];
	DIR *dp;
	struct dirent *de;
	int found = 0, fd, e;
	struct stat sb;

	pc_begin("XBD.4.16-pathname-resolution.004");
	pc_make_dir("d");
	max = pathconf("d", _PC_NAME_MAX);
	if (max < 0)
		pc_notinuse("{NAME_MAX} has no limit here");
	if (max > 4000)
		pc_notinuse("{NAME_MAX} is %ld here: too large to try", max);
	pc_test();
	memset(name, 'n', (size_t)max);
	name[max] = '\0';
	(void)snprintf(p, sizeof p, "d/%s", name);
	fd = open(p, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "creating a file whose name has {NAME_MAX} = %ld bytes failed: %s", max, pc_errname(errno));
	(void)close(fd);
	pc_check(stat(p, &sb) == 0, "stat of the {NAME_MAX}-byte name failed: %s", pc_errname(errno));
	dp = opendir("d");
	pc_check(dp != NULL, "opendir failed");
	while ((de = readdir(dp)) != NULL)
		if (strcmp(de->d_name, name) == 0)
			found++;
	(void)closedir(dp);
	pc_check(found == 1, "the {NAME_MAX}-byte name is listed %d times", found);
	pc_check(unlink(p) == 0, "unlink of the {NAME_MAX}-byte name failed");

	name[max] = 'n';
	name[max + 1] = '\0';
	(void)snprintf(p, sizeof p, "d/%s", name);
	fd = open(p, O_WRONLY | O_CREAT | O_EXCL, 0600);
	e = errno;
	pc_check(fd == -1, "creating a file whose name has {NAME_MAX}+1 bytes succeeded");
	pc_check(e == ENAMETOOLONG, "creating a file whose name has {NAME_MAX}+1 bytes failed with %s, expected ENAMETOOLONG", pc_errname(e));
	pc_check(mkdir(p, 0700) == -1 && errno == ENAMETOOLONG, "mkdir of a name of {NAME_MAX}+1 bytes did not fail with ENAMETOOLONG (%s)", pc_errname(errno));
	stat_fails(p, ENAMETOOLONG, ENOENT);
	(void)snprintf(p, sizeof p, "d/%s/x", name);
	stat_fails(p, ENAMETOOLONG, ENOENT);
	(void)snprintf(p, sizeof p, "d/%s", name);
	name[max] = '\0';
	(void)snprintf(p, sizeof p, "d/%s", name);
	pc_check(!exists(p), "a name was created from {NAME_MAX}+1 bytes (shortened to {NAME_MAX})");
	dp = opendir("d");
	pc_check(dp != NULL, "opendir failed");
	while ((de = readdir(dp)) != NULL)
		if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
			found += 100;
	(void)closedir(dp);
	pc_check(found == 1, "the directory holds entries after the failed creations");
	pc_pass();
}
