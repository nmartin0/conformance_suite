/*
 * A file just created has all three timestamps within a second of the
 * clock (no further back than the resolution allows, none ahead of it),
 * with valid nanosecond fields, and so does the directory it was created in.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.12-file-times-update.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

static long long ns(struct timespec t)
{
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static void within(const char *what, struct timespec t, struct timespec lo, struct timespec hi)
{
	pc_check(t.tv_nsec >= 0 && t.tv_nsec < 1000000000L, "%s has tv_nsec %ld", what, (long)t.tv_nsec);
	pc_check(ns(t) >= ns(lo) - 1000000000LL, "%s is %lld ns before the call, further back than one second", what, ns(lo) - ns(t));
	pc_check(ns(t) <= ns(hi), "%s is %lld ns after the end of the checks", what, ns(t) - ns(hi));
}

int main(void)
{
	struct timespec t0, t1;
	struct stat f, d;
	int fd;

	pc_begin("XBD.4.12-file-times-update.001");
	pc_make_dir("dir");
	pc_test();
	(void)clock_gettime(CLOCK_REALTIME, &t0);
	fd = open("dir/new", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "creating the file failed: %s", pc_errname(errno));
	pc_check(fstat(fd, &f) == 0, "fstat failed: %s", pc_errname(errno));
	pc_check(stat("dir", &d) == 0, "stat of the directory failed: %s", pc_errname(errno));
	(void)clock_gettime(CLOCK_REALTIME, &t1);
	(void)close(fd);
	within("st_atim of the new file", f.st_atim, t0, t1);
	within("st_mtim of the new file", f.st_mtim, t0, t1);
	within("st_ctim of the new file", f.st_ctim, t0, t1);
	within("st_mtim of the directory", d.st_mtim, t0, t1);
	within("st_ctim of the directory", d.st_ctim, t0, t1);
	pc_pass();
}
